#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <math.h>

struct item {
    int weight;
    int value;
};

struct line {
    int values[28];
    int weights[28];
    int amount;
    int idp;
};

/* the number of threads */
int num_threads;
int item_count = 0;
int weight_limit = 0;
struct item items[28];
struct line starting_lines[16];

void add1(int *a, int len) {
    int carry = 1;
    for (int i = len - 1; carry > 0 && i >= 0; i--) {
        int result = a[i] + carry;
        carry = result >> 1;
        a[i] = result & 1;
    }
}

/* the function called for each thread */
void* func(void* line_p) {
    struct line starting_line = *(struct line*) line_p;
    /* get our thread id */
    int id = starting_line.idp;
    int line[item_count];
    int temp[starting_line.amount];
    /* allocate space for the answer */
    int* best = malloc(sizeof(int));
    int weight = 0;

    if(starting_line.weights[id] <= weight_limit) {
        *best = starting_line.values[id];
        weight += starting_line.weights[id];
    }
    printf("best: %d\n", (int) *best);
    /* calculate the best combination based on file */
    for(int i = 0; i < starting_line.amount; i++) {
        for(int j = 0; j < item_count ; j++) { 
            add1(line, item_count);
            //printf("line[%d]: %d\n", j, line[j]);
            //possibilities[i][j] = line[j];
            temp[j] = line[j];
            printf("temp[%d] = %d\n", j, temp[j]); 
            
            if((*best <= starting_line.values[temp[j]] + *best) && (weight + starting_line.weights[temp[j]] <= weight_limit)) {
                *best = starting_line.values[temp[j]];
                printf("new best: %d\n", *best);
            }
            //printf("[%d][%d] = %d ", i, j, possibilities[i][j]);
        }
        //printf("line[%d]: %d\n", i, line[j]);
        //possibilities[i][j] = line[i];
        temp[i] = line[i];
        if(*best < starting_line.values[temp[i]])
            *best = starting_line.values[temp[i]];

        //printf("\n");
    }   

    /* debugging output */
    // printf("Thread %d: sum(%d, %d) = %d\n", id, start, end, *answer);

    return best;
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
        //printf("filename: %s\n", filename);

        num_threads = atoi(argv[2]);
        //printf("threads: %d\n", num_threads);
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
    int i, j, temp;
    int total_combos = pow(2, item_count);
    //printf("%d",total_combos); 
    int amt_per_thread = total_combos / num_threads;    
    //int possibilities[(int)total_combos][item_count];
    int line[item_count];

    for(i = 0; i < total_combos; i++) {
        for(j = 0; j < item_count; j++) {
            //possibilities[i][j] = 0;
        }
        line[i] = 0;
    }

    /* spawn all threads */
    for (i = 0; i < num_threads; i++) {
        for(j = 0; j < amt_per_thread; j++) {
            add1(line, item_count); 
        }
        starting_lines[i].values[i] = items[line[i]].value;
        starting_lines[i].weights[i] = items[line[i]].weight;
        starting_lines[i].amount = amt_per_thread;
        pthread_create(&threads[i], NULL, func, &starting_lines[i]);
        ids[i] = i;
    }

    /* join all threads collecting answer */
    int best = 0;
    for (i = 0; i < num_threads; i++) {
        int* result;
        pthread_join(threads[i], (void**) &result);
        if(best < *result)
            best = *result;
        free(result);
    }

    /* now all results are in */
    printf("Final value is = %d.\n", best);
    pthread_exit(NULL);
}
