#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int burst_time;
    int priority;
} Process;

#define TIME_QUANTUM 1

void schedule_priority_rr(Process *processes, int n) {
    int *remaining_times = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        remaining_times[i] = processes[i].burst_time;
    }

    // Sort processes by priority
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[j].priority > processes[i].priority) {
                // Swap all attributes at once
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;

                // Update remaining times after swapping
                int temp_remaining = remaining_times[i];
                remaining_times[i] = remaining_times[j];
                remaining_times[j] = temp_remaining;
            }
        }
    }

    // Group processes by priority
    int current_priority = processes[0].priority;
    int group_start = 0;

    while (1) {
        int done = 1;

        for (int i = group_start; i < n; i++) {
            if (processes[i].priority != current_priority) {
                break;
            }

            if (remaining_times[i] > 0) {
                done = 0;

                if (remaining_times[i] > TIME_QUANTUM) {
                    printf("Process %d (priority %d) runs for %d units\n", processes[i].pid, processes[i].priority, TIME_QUANTUM);
                    remaining_times[i] -= TIME_QUANTUM;
                } else {
                    printf("Process %d (priority %d) runs for %d units\n", processes[i].pid, processes[i].priority, remaining_times[i]);
                    remaining_times[i] = 0;
                }
            }
        }

        if (done == 1) {
            group_start += 1;

            if (group_start >= n) {
                break;
            }

            current_priority = processes[group_start].priority;
        }
    }

    free(remaining_times);
}

int main() {
    Process processes[] = {
        {1, 24, 2},
        {2, 3, 1},
        {3, 3, 3},
        {4, 5, 2}
    };

    int n = sizeof(processes) / sizeof(Process);

    schedule_priority_rr(processes, n);

    return 0;
}