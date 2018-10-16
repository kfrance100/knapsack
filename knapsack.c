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
    int idp;
};

/* the number of threads */
int num_threads;
int item_count = 0;
int weight_limit = 0;
struct item items[28];
struct line starting_lines[16];
pthread_mutex_t lock;

void add1(int *Line, int itemCount) {
    int carry = 1;
    for (int i = itemCount - 1; carry > 0 && i >= 0; i--) {
        int result = Line[i] + carry;
        carry = result >> 1;
        Line[i] = result & 1;
    }
}

/* the function called for each thread */
void* func(void* startingLine) {
    struct line starting_line = *(struct line*) startingLine;
    /* get our thread id */
    int id = starting_line.idp;
    int line[item_count];
        
    /* allocate space for the answer */
    int* best = malloc(sizeof(int));
    //best = 0;
    int weight, value;
    int total_combos = pow(2, item_count);
    int amt_per_thread = total_combos / num_threads;
    int temp[amt_per_thread];


    //this may be wrong
    /*if(starting_line.weights[id] <= weight_limit) {
        *best = starting_line.values[id];
        weight += starting_line.weights[id];
    } */
    //printf("best: %d, thread: %d\n", (int) *best, id);
    
    pthread_mutex_lock(&lock);
    /* calculate the best combination based on file */
    for(int i = 0; i < amt_per_thread; i++) {
        for(int j = 0; j < item_count ; j++) { 
            add1(line, item_count);
            //printf("line: %d\n", line[j]);
            temp[j] = line[j];
            starting_line.values[j] = items[j].value;
            starting_line.weights[j] = items[j].weight;
            weight = starting_line.weights[j];
            value = starting_line.values[j];
            //printf("temp[%d] = %d\n", j, temp[j]); 
            
            if((temp[j] == 1) && (starting_line.values[j] + value >= *best) && (weight + starting_line.weights[j] <= weight_limit)) {
                *best = *best + starting_line.values[j];
                printf("new best: %d value: %d\n", *best, starting_line.values[j]);
            }
        }
        //printf("\n");
        //printf("line[%d]: %d\n", i, line[j]);
        /*temp[i] = line[i];
        if(*best < starting_line.values[i])
            *best = starting_line.values[i];
        */
        //printf("\n");
    }
    pthread_mutex_unlock(&lock);

    /* debugging output */
    // printf("Thread %d: sum(%d, %d) = %d\n", id, start, end, *answer);

    return best;
}

int main (int argc, char** argv) {
    char *filename;
    int x = 0;
    int y = 0;

    pthread_mutex_init(&lock, NULL);


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
    int line[item_count];
        
    for(i = 0; i < item_count; i++) {
        line[i] = 0;
    }

    /* spawn all threads */
    for (i = 0; i < num_threads; i++) {
        ids[i] = i;
        //add1(line, item_count);
        starting_lines[i].weights[i] = items[i].weight;
        starting_lines[i].values[i] = items[i].value;
        //printf("weights[%d]: %d \n", i, starting_lines[i].weights[i]);
        //printf("values[%d]: %d \n",i, starting_lines[i].values[i]);
        starting_lines[i].idp = ids[i];
        //printf("thread #: %d\n", starting_lines[i].idp); 
        pthread_create(&threads[i], NULL, func, &starting_lines[i]);
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
