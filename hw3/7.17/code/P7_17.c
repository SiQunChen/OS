#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS 4
#define NUM_POINTS 1000000

static long long num_inside_circle = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

double random_double() {
    return (double)rand() / RAND_MAX;
}

void *monte_carlo_thread(void *arg) {
    long long local_count = 0;
    int i;

    for (i = 0; i < NUM_POINTS / NUM_THREADS; i++) {
        double x = random_double();
        double y = random_double();
        double distance = sqrt(x * x + y * y);

        if (distance <= 1.0) {
            local_count++;
        }
    }

    pthread_mutex_lock(&mutex);
    num_inside_circle += local_count;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t threads[NUM_THREADS];
    int i;

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, monte_carlo_thread, NULL);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    double pi_estimate = 4.0 * num_inside_circle / (double)NUM_POINTS;
    printf("Estimated value of pi: %lf\n", pi_estimate);

    return 0;
}