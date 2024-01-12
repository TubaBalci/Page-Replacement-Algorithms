#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

#define MAX_LINE_LENGTH 100

int N; // memory size
int arrSize = 0; // array size 

int file_read();
int FIFO(int*);
int LRU(int*);
int LFU(int*);
int n_random(int*);
int w_random(int*);


int main(int argc, char ** argv)
{
    N = atoi(argv[1]);
    if(file_read()==1)
        printf("error occured");
    return 0;
}


int file_read(){
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        //fgets(line, sizeof(line), file);
        line[strcspn(line, "\n")] = '\0';

        // Split the line into integers
        int line_content[MAX_LINE_LENGTH];
        int count = 0;

        // Tokenize and copy directly to the array
        char *token = strtok(line, ", ");
        while (token != NULL) {
            line_content[count++] = atoi(token);
            token = strtok(NULL, ", ");
        }

        arrSize = count;
        // Allocate memory for the copy
        int* access_list = (int*)malloc(count * sizeof(int));
        if (access_list == NULL) {
            perror("Memory allocation failed");
            return 1;
        }

        // Copy contents of the array to allocated memory
        memcpy(access_list, line_content, count * sizeof(int));

        printf("Sayfa Erisim Listesi: ");
        for(int i =0; i< arrSize; i++)
            printf("%d ",access_list[i]);
        printf("\n");    
        
        FIFO(access_list);
        LRU(access_list);
        LFU(access_list);
        n_random(access_list);
        w_random(access_list);
        printf("--------------------------------\n");

        // Free allocated memory
        free(access_list);
    }
    fclose(file);
}

int FIFO(int* access_list){
    int time = 0;
    int miss_count = 0;
    struct node {
        int page;// page index number
        int in_time;// time the page brought into the memory
    };

    typedef struct node node;
    node* memory = (node*)malloc(N * sizeof(node));
    for(int j = 0; j < N; j++){
            memory[j].page = -1;
            memory[j].in_time = -1;
    }
    
    for(int i = 0; i < arrSize; i++,time++){
        int located = 0;
        for(int j = 0; j < N && !located; j++){
            if (memory[j].page == access_list[i]){//if page exists in memory already
                
                located = 1;
            }
            else if(memory[j].in_time == -1){// if there is a space in memory
                miss_count++;
                memory[j].page = access_list[i];
                memory[j].in_time = time;
                located = 1;
            }           
        }
        if(!located){// if there is no space or the page doesnt exists in memory, page fault state 
            miss_count++;
            int min_time = memory[0].in_time, min_time_index = 0;
            for(int k = 1; k < N; k++){
                if(memory[k].in_time < min_time){
                    min_time = memory[k].in_time;
                    min_time_index = k;
                }
            }
            memory[min_time_index].page = access_list[i];
            memory[min_time_index].in_time = time;
        }
    }

    free(memory);
    printf("FIFO: %d\n", miss_count);
}

int LRU(int* access_list){
    int time = 1;
    int miss_count = 0;
    struct node {
        int page;// page index number
        int access_time;// time the page last used 
    };
    typedef struct node node;
    node* memory = (node*)malloc(N * sizeof(node));
    for(int j = 0; j < N; j++){
            memory[j].page = -1;
            memory[j].access_time = -1;
    }
    
    for(int i = 0; i < arrSize; i++,time++){
        int located = 0;
        for(int j = 0; j < N && !located; j++){
            if (memory[j].page == access_list[i]){//if page exists in memory already
                memory[j].access_time = time;// updating access time
                located = 1;
            }
            else if(memory[j].access_time == -1){// if there is a space in memory
                memory[j].page = access_list[i];
                memory[j].access_time = time;
                miss_count++;
                located = 1;
            }           
        }
        if(!located){
            miss_count++;
            //to replace current page with least recently used one
            int min_access_time = memory[0].access_time, min_time_index = 0;
            for(int k = 1; k < N; k++){
                if(memory[k].access_time < min_access_time){
                    min_access_time = memory[k].access_time;
                    min_time_index = k;
                }
            }
            memory[min_time_index].page = access_list[i];
            memory[min_time_index].access_time = time;
        }       
    }

    free(memory);
    printf("LRU: %d\n", miss_count);

}

int LFU(int* access_list){
    int time = 1;
    int miss_count = 0;
    struct node {
        int page;// page index number
        int access_count;// how many times a page used
        int access_time;// time the page last used 
    };
    typedef struct node node;
    node* memory = (node*)malloc(N * sizeof(node));
    for(int j = 0; j < N; j++){
            memory[j].page = -1;
            memory[j].access_count = -1;
            memory[j].access_time = -1;            
    }
    
    for(int i = 0; i < arrSize; i++,time++){
        int located = 0;
        for(int j = 0; j < N && !located; j++){
            if (memory[j].page == access_list[i]){//if page exists in memory already
                memory[j].access_count++; //update access count 
                memory[j].access_time = time; //update access time
                located = 1;
            }
            else if(memory[j].access_count == -1){// if there is a space in memory
                memory[j].page = access_list[i];
                memory[j].access_count = 0;
                memory[j].access_time = time;
                miss_count++;
                located = 1;
            }           
        }
        if(!located){
            miss_count++;
            //to replace current page with the page has the first in time
            int min_access = memory[0].access_count, min_access_index = 0;
            for(int k = 1; k < N; k++){
                if(memory[k].access_count < min_access 
                || memory[k].access_count == min_access && memory[k].access_time < memory[min_access_index].access_time)
                {
                    min_access = memory[k].access_count;
                    min_access_index = k;
                }
            }
            memory[min_access_index].page = access_list[i];
            memory[min_access_index].access_count = 0;
            memory[min_access_index].access_time = time;
        }
    }

    free(memory);
    printf("LFU: %d\n", miss_count);

}

int n_random(int* access_list){
    srand(time(NULL));
    int miss_count = 0;
    int* memory = (int*)malloc(N * sizeof(int));
    
    for(int z = 0 ; z < 1000; z++)
    {
        //reset memory after each iteration
        for(int j = 0; j < N; j++)
            memory[j] = -1;            

        for(int i = 0; i < arrSize; i++){
           int located = 0;
           for(int j = 0; j < N && !located; j++){
               if (memory[j]== access_list[i])//if page exists in memory already
                   located = 1;
               else if(memory[j] == -1){// if there is a space in memory
                   memory[j] = access_list[i];
                   miss_count++;
                   located = 1;
               }           
           }
           if(!located){
               //to replace the called page with a random location
               int rand_index = rand() % N;               
               memory[rand_index]= access_list[i];
               miss_count++;
           }
        }
    }

    free(memory);
    printf("Random: %f\n", (double)(miss_count)/1000);

}


int w_random(int* access_list){
    srand(time(NULL));
    int miss_count = 0;
    struct node {
        int page;// page index number
        int access_count;// how many times a page used
    };

    typedef struct node node;
    node* memory = (node*)malloc(N * sizeof(node));

    for(int z = 0 ; z < 1000; z++)
    {
        //reset memory after each iteration
        for(int j = 0; j < N; j++){
            memory[j].page = -1;
            memory[j].access_count = -1;       
        }
        for(int i = 0; i < arrSize; i++){
           int located = 0;
           for(int j = 0; j < N && !located; j++){
               if (memory[j].page== access_list[i])//if page exists in memory already
                   {
                    located = 1;
                    memory[j].access_count++;
                   }
               else if(memory[j].page == -1){// if there is a space in memory
                   memory[j].page = access_list[i];
                   memory[j].access_count = 1;
                   miss_count++;
                   located = 1;
               }           
           }
           if(!located){
                miss_count++;
               //to replace current page with a weighted random location
               float total_weight=0;

                float weight [N];    
                for(int k = 0; k < N; k++){
                    weight [k] = (float)1/memory[k].access_count; 
                    total_weight += weight [k];     
                    }

               int rand_index = rand() % (int)(total_weight*100);
               
               for(int k = N-1; k >= 0; k--){             
                    rand_index -= weight[k] * 100;
                    if(rand_index < 0){
                        memory[k].page = access_list[i];
                        memory[k].access_count = 1;
                        break;
                    }
               }
               
           }
        }
    }

    free(memory);
    printf("Weighted random: %f\n", (double)(miss_count)/1000);
}
