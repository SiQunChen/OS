#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int sequence_length;
long fib_new_number;

void *run(void *param) {
    int i, n = atoi(param);
    long fib_1 = 0, fib_2 = 1;

    pthread_mutex_lock(&mutex);
    fib_new_number = fib_1;
    sequence_length = 1;
    pthread_cond_signal(&cond);
    pthread_cond_wait(&cond, &mutex);

    fib_new_number = fib_2;
    sequence_length = 2;
    pthread_cond_signal(&cond);
    pthread_cond_wait(&cond, &mutex);

    for (i = 2; i < n; i++) {
        fib_new_number = fib_1 + fib_2;
        fib_1 = fib_2;
        fib_2 = fib_new_number;
        sequence_length = i + 1;
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond, &mutex);
    }

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t tid;
    pthread_attr_t attr;
    int i;

    if (argc != 2) {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, run, argv[1]);

    pthread_mutex_lock(&mutex);
    for (i = 0; i < atoi(argv[1]); i++) {
        pthread_cond_wait(&cond, &mutex);
        printf("%ld\n", fib_new_number);
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);

    pthread_join(tid, NULL);
    return 0;
}