#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

/* the number of threads */
int num_threads;

/* the function called for each thread */
void* func(void* idp) {
    /* get our thread id */
    int id = * (int*) idp;

    /* calculate the best combination based on file */

    /* allocate space for the answer */
    int* answer = malloc(sizeof(int));

    /* debugging output */
    // printf("Thread %d: sum(%d, %d) = %d\n", id, start, end, *answer);

    return answer;
}

int main (int argc, char** argv) {
    char *filename;
    int nums[30] = {0};
    int x, y = 0;
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
    
    if (file == NULL){
        printf("Error! opening file");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }

    while ((x = getc(file)) != EOF) {
        putchar(x);
        nums[y] = x;
        //printf("\nnums[%d] = %d\n", y, x);
        y = y + 1;
    }

    /* close the file */    
    fclose(file);
   
    /* an array of threads */
    pthread_t threads[num_threads];
    int ids[num_threads];
    int i;

    /* spawn all threads */
    for (i = 0; i < num_threads; i++) {
        ids[i] = i;
        //pthread_create(&threads[i], NULL, i, &ids[i]);
    }

    /* join all threads collecting answer */
    int answer = 0;
    for (i = 0; i < num_threads; i++) {
        int* partial;
        //pthread_join(threads[i], (void**) &partial);
        //answer += *partial;
        //free(partial);
    }

    /* now all results are in */
    //printf("Final answer = %d.\n", answer);
    pthread_exit(NULL);
}
