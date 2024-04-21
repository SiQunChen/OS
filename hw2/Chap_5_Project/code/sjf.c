#include <stdio.h>

typedef struct {
    int pid;
    int burst_time;
} Process;

void schedule_sjf(Process *processes, int n) {
    int wait_time = 0;
    int turnaround_time = 0;

    // Sort processes by burst time
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[j].burst_time < processes[i].burst_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    printf("Process\tBurst Time\tWait Time\tTurnaround Time\n");

    for (int i = 0; i < n; i++) {
        turnaround_time = wait_time + processes[i].burst_time;

        printf("%d\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].burst_time, wait_time, turnaround_time);

        wait_time += processes[i].burst_time;
    }
}

int main() {
    Process processes[] = {
        {1, 24},
        {2, 3},
        {3, 3}
    };

    int n = sizeof(processes) / sizeof(Process);

    schedule_sjf(processes, n);

    return 0;
}
