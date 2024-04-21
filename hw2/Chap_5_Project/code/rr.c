#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int burst_time;
} Process;

#define TIME_QUANTUM 1

void schedule_rr(Process *processes, int n) {
    int *remaining_times = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        remaining_times[i] = processes[i].burst_time;
    }

    while (1) {
        int done = 1;

        for (int i = 0; i < n; i++) {
            if (remaining_times[i] > 0) {
                done = 0;

                if (remaining_times[i] > TIME_QUANTUM) {
                    printf("Process %d runs for %d units\n", processes[i].pid, TIME_QUANTUM);
                    remaining_times[i] -= TIME_QUANTUM;
                } else {
                    printf("Process %d runs for %d units\n", processes[i].pid, remaining_times[i]);
                    remaining_times[i] = 0;
                }
            }
        }

        if (done == 1) {
            break;
        }
    }

    free(remaining_times);
}

int main() {
    Process processes[] = {
        {1, 24},
        {2, 3},
        {3, 3}
    };

    int n = sizeof(processes) / sizeof(Process);

    schedule_rr(processes, n);

    return 0;
}
