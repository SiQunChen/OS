#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define NUM_FARMERS 10

sem_t bridge;
sem_t north_sem, south_sem;

pthread_mutex_t mutex;

int north_count = 0, south_count = 0;

void cross_bridge(int direction) {
    int sleep_time = rand() % 5 + 1;
    sleep(sleep_time);
    printf("Farmer from %s crossed the bridge in %d seconds.\n", (direction == 0) ? "North" : "South", sleep_time);
}

void *north_farmer(void *arg) {
    sem_wait(&north_sem); 
    pthread_mutex_lock(&mutex);
    north_count++;

    if (north_count == 1 && south_count == 0) {
        sem_wait(&bridge); 
    }

    pthread_mutex_unlock(&mutex); 
    sem_post(&north_sem); 

    cross_bridge(0); 

    pthread_mutex_lock(&mutex); 
    north_count--;

    if (north_count == 0 && south_count == 0) {
        sem_post(&bridge); 
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *south_farmer(void *arg) {
    sem_wait(&south_sem); 
    pthread_mutex_lock(&mutex); 
    south_count++;

    if (south_count == 1 && north_count == 0) {
        sem_wait(&bridge);
    }

    pthread_mutex_unlock(&mutex); 
    sem_post(&south_sem); 

    cross_bridge(1);

    pthread_mutex_lock(&mutex); 
    south_count--;

    if (north_count == 0 && south_count == 0) {
        sem_post(&bridge); 
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    srand(time(NULL)); 

    sem_init(&bridge, 0, 1);
    sem_init(&north_sem, 0, 1);
    sem_init(&south_sem, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    pthread_t farmers[NUM_FARMERS];

    for (int i = 0; i < NUM_FARMERS; i++) {
        if (i % 2 == 0) {
            pthread_create(&farmers[i], NULL, north_farmer, NULL);
        } else {
            pthread_create(&farmers[i], NULL, south_farmer, NULL);
        }
    }

    for (int i = 0; i < NUM_FARMERS; i++) {
        pthread_join(farmers[i], NULL);
    }

    sem_destroy(&bridge);
    sem_destroy(&north_sem);
    sem_destroy(&south_sem);
    pthread_mutex_destroy(&mutex);

    return 0;
}