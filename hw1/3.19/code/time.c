#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/shm.h>

// 定義共享記憶體的大小
#define SHM_SIZE  sizeof(struct timeval)

// 計算時間差，並打印結果
void calculateElapsedTime(struct timeval start, struct timeval end) {
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }
    printf("Elapsed time: %ld seconds and %ld microseconds\n", seconds, microseconds);
}

// 使用共享記憶體作為IPC機制
void useSharedMemory(char* command) {
    int shmid;
    struct timeval* shared_time;
    pid_t pid;

    // 創建共享記憶體
    if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // 附加共享記憶體到進程的地址空間
    if ((shared_time = shmat(shmid, NULL, 0)) == (struct timeval *) -1) {
        perror("shmat");
        exit(1);
    }

    // 創建子進程
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // 子進程
        gettimeofday(shared_time, NULL); // 寫入開始時間到共享記憶體
        execlp(command, command, NULL); // 執行命令
        perror("execlp"); // 如果執行失敗，則顯示錯誤訊息
        exit(1);
    } else { // 父進程
        wait(NULL); // 等待子進程結束
        struct timeval end_time;
        gettimeofday(&end_time, NULL); // 獲取結束時間
        calculateElapsedTime(*shared_time, end_time); // 計算並打印執行時間
        // 釋放共享記憶體
        if (shmdt(shared_time) == -1) {
            perror("shmdt");
            exit(1);
        }
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }
    }
}

// 使用管道作為IPC機制
void usePipe(char* command) {
    int pipefd[2];
    pid_t pid;
    struct timeval start_time;

    // 創建管道
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    // 創建子進程
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // 子進程
        gettimeofday(&start_time, NULL); // 獲取開始時間
        // 將開始時間寫入管道
        if (write(pipefd[1], &start_time, sizeof(start_time)) == -1) {
            perror("write");
            exit(1);
        }
        close(pipefd[1]); // 關閉管道的寫端
        execlp(command, command, NULL); // 執行命令
        perror("execlp"); // 如果執行失敗，則顯示錯誤訊息
        exit(1);
    } else { // 父進程
        wait(NULL); // 等待子進程結束
        struct timeval end_time;
        gettimeofday(&end_time, NULL); // 獲取結束時間
        // 從管道讀取開始時間
        if (read(pipefd[0], &start_time, sizeof(start_time)) == -1) {
            perror("read");
            exit(1);
        }
        close(pipefd[0]); // 關閉管道的讀端
        calculateElapsedTime(start_time, end_time); // 計算並打印執行時間
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(1);
    }

    printf("Using shared memory:\n");
    useSharedMemory(argv[1]); // 使用共享記憶體作為IPC機制
    printf("\n");

    printf("Using pipe:\n");
    usePipe(argv[1]); // 使用管道作為IPC機制

    return 0;
}
