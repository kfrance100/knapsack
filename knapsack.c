#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

/* the number of threads */
int num_threads;
#define START 0
#define END 10000

/* the function called for each thread */
void* sum_part(void* idp) {
    /* get our thread id */
    int id = * (int*) idp;

    /* calculate the start and end points by evenly dividing the range */
    int start = ((END - START) / num_threads) * id;
    int end = start + ((END - START) / num_threads) - 1;

    /* the last thread needs to do all remaining ones */
    if (id == (num_threads - 1)) {
        end = END;
    }

    /* allocate space for the answer */
    int* answer = malloc(sizeof(int));

    /* do the calculation */
    int i;
    for (i = start; i <= end; i++) {
        *answer += i;
    }

    /* debugging output */
    printf("Thread %d: sum(%d, %d) = %d\n", id, start, end, *answer);

    return answer;
}

int main (int argc, char** argv) {
    /* get the number of threads */
    if (argc < 2) {
        printf("Pass the number of threads to use!\n");
        return 0;
    } else {
        num_threads = atoi(argv[1]);
    }

    /* an array of threads */
    pthread_t threads[num_threads];
    int ids[num_threads];
    int i;

    /* spawn all threads */
    for (i = 0; i < num_threads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, sum_part, &ids[i]);
    }

    /* join all threads collecting answer */
    int answer = 0;
    for (i = 0; i < num_threads; i++) {
        int* partial;
        pthread_join(threads[i], (void**) &partial);
        answer += *partial;
        free(partial);
    }

    /* now all results are in */
    printf("Final answer = %d.\n", answer);
    pthread_exit(NULL);
}
