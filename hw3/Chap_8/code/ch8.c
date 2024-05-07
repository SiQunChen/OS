#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_RESOURCES 3
#define NUM_CUSTOMERS 5

int available[NUM_RESOURCES];
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];
int need[NUM_CUSTOMERS][NUM_RESOURCES];
bool finish[NUM_CUSTOMERS];
bool terminate = false; // Global variable to indicate termination condition

pthread_mutex_t mutex;
pthread_cond_t condition;

void *customer(void *arg) {
    int id = *(int *)arg;

    while (!terminate) { // Check termination condition
        pthread_mutex_lock(&mutex);

        // Generate random request
        int request[NUM_RESOURCES];
        for (int i = 0; i < NUM_RESOURCES; i++) {
            request[i] = rand() % (need[id][i] + 1);
        }

        // Check if request can be granted
        bool safe = true;
        for (int i = 0; i < NUM_RESOURCES; i++) {
            if (request[i] > available[i]) {
                safe = false;
                break;
            }
        }

        // Grant request if safe
        if (safe) {
            for (int i = 0; i < NUM_RESOURCES; i++) {
                available[i] -= request[i];
                allocation[id][i] += request[i];
                need[id][i] -= request[i];
            }
            printf("Customer %d's request granted\n", id);
        } else {
            printf("Customer %d's request denied\n", id);
        }

        pthread_mutex_unlock(&mutex);

        // Simulate some work before next request
        sleep(1);
    }

    printf("Customer %d is terminating\n", id);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // Initialize available resources from command line arguments
    for (int i = 1; i <= NUM_RESOURCES; i++) {
        available[i - 1] = atoi(argv[i]);
    }

    // Initialize maximum resources randomly for each customer
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            maximum[i][j] = rand() % (available[j] + 1);
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
        }
        finish[i] = false;
    }

    // Initialize mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);

    // Create customer threads
    pthread_t customers[NUM_CUSTOMERS];
    int customer_ids[NUM_CUSTOMERS];
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_ids[i] = i;
        pthread_create(&customers[i], NULL, customer, &customer_ids[i]);
    }

    // Simulate running for a period of time
    sleep(10); // Change this value to the desired duration of the program

    // Set termination condition
    terminate = true;

    // Wait for all customer threads to finish
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }

    // Cleanup
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

    return 0;
}
