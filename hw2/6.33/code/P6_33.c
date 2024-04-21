#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;
pthread_mutex_t mutex;

int decrease_count(int count) {
    pthread_mutex_lock(&mutex);
    if (available_resources < count) {
        pthread_mutex_unlock(&mutex);
        return -1;
    } else {
        available_resources -= count;
        pthread_mutex_unlock(&mutex);
        return 0;
    }
}

int increase_count(int count) {
    pthread_mutex_lock(&mutex);
    available_resources += count;
    pthread_mutex_unlock(&mutex);
    return 0;
}

void *thread_func(void *arg) {
    long count = (long)arg;
    if (decrease_count(count) == 0) {
        printf("Thread %lu acquired %ld resources, available resources: %d\n", pthread_self(), count, available_resources);
        sleep(1);  // Simulate work
        increase_count(count);
        printf("Thread %lu released %ld resources, available resources: %d\n", pthread_self(), count, available_resources);
    } else {
        printf("Thread %lu failed to acquire %ld resources, available resources: %d\n", pthread_self(), count, available_resources);
    }
    return NULL;
}

int main() {
    pthread_t threads[MAX_RESOURCES + 1];
    pthread_mutex_init(&mutex, NULL);

    for (long i = 0; i <= MAX_RESOURCES; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *)i);
    }

    for (int i = 0; i <= MAX_RESOURCES; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
