#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long *fib_seq;

void *run(void *param) {
    int i, n = atoi(param);
    fib_seq = (long *)malloc(n * sizeof(long));
    fib_seq[0] = 0;
    if (n > 1) {
        fib_seq[1] = 1;
        for (i = 2; i < n; i++)
            fib_seq[i] = fib_seq[i - 1] + fib_seq[i - 2];
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    pthread_t tid;
    pthread_attr_t attr;

    if (argc != 2) {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, run, argv[1]);
    pthread_join(tid, NULL);

    for (int i = 0; i < atoi(argv[1]); i++)
        printf("%ld\n", fib_seq[i]);

    return 0;
}
