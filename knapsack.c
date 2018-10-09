#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

struct item {
    int weight;
    int value;
};


/* the number of threads */
int num_threads;
int item_count = 0;
int weight_limit = 0;

struct item items[50];


/* the function called for each thread */
void* func(void* idp) {
    /* get our thread id */
    int id = * (int*) idp;
    
    int possibilities = 2 ^ item_count;
    int amtPerThread = possibilities / num_threads;
    

    /* calculate the best combination based on file */
    //printf("item 1:%d\n", items[1].value);

    /* allocate space for the answer */
    int* answer = malloc(sizeof(int));

    /* debugging output */
    // printf("Thread %d: sum(%d, %d) = %d\n", id, start, end, *answer);

    return answer;
}

int main (int argc, char** argv) {
    char *filename;
    int x = 0;
    int y = 0;
    
    /* get the number of threads */
    if (argc < 3 || argv[2] < 0) {
        printf("Pass the knapsack file name and the number of threads to run the program with!\n");
        return 0;
    } else { 
        filename = argv[1];
        printf("filename: %s\n", filename);
         
        num_threads = atoi(argv[2]);
        printf("threads: %d\n", num_threads);
    }

    /* read in file */
    FILE *file = fopen(filename, "r");
    
    /* catches file name error */
    if (file == NULL){
        printf("Error! Incorrect file name.\n");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    
    /* reads numbers from file, keeps track of number of items, stores item values in array of structs */
    fscanf(file, "%d", &weight_limit);
    while (fscanf(file, "%d %d", &items[y].weight, &items[y].value) != EOF) {
        item_count = item_count + 1;
        //x = getc(file); 
        //putchar(x);
        //printf("item[%d]: %d, %d\n", y, items[y].weight, items[y].value);
        //printf("count: %d\n", item_count);
        y = y + 1;
    }

    /* close the file */    
    fclose(file);
   
    /* an array of threads */
    pthread_t threads[num_threads];
    int ids[num_threads];
    int i, j;
    int total_combos = 2 ^ item_count;
    int amt_per_thread = total_combos / num_threads;    
    int possibilities[total_combos][item_count];

    /* set possibilites matrix */
    for(i = 0; i <= total_combos; i++) {
        for(j = 0; j <= item_count; j++) { 
            possibilities[i][j] = 1;
            printf("possibilities[%d][%d] = %d\n", i, j, possibilities[i][j]);
        }
    }
    /* spawn all threads */
    for (i = 0; i < num_threads; i++) {
        for(j = 0; j < amt_per_thread; j++) {
            pthread_create(&threads[i], NULL, func, &items[j]);
        }
        ids[i] = i;
    }

    /* join all threads collecting answer */
    int answer = 0;
    for (i = 0; i < num_threads; i++) {
        //int* partial;
        //pthread_join(threads[i], NULL, func, items[]);
        //answer += *partial;
        //free(partial);
    }

    /* now all results are in */
    //printf("Final answer = %d.\n", answer);
    pthread_exit(NULL);
}
