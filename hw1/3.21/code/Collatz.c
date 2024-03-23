#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// 生成 Collatz 序列
void generateCollatzSequence(int n) {
    printf("%d, ", n);
    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        printf("%d, ", n);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <starting_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 檢查參數是否是正整數
    int starting_number = atoi(argv[1]);
    if (starting_number <= 0) {
        fprintf(stderr, "Please provide a positive integer as the starting number.\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // 子進程
        generateCollatzSequence(starting_number);
    } else { // 父進程
        wait(NULL); // 等待子進程完成
        printf("Child process has completed.\n");
    }

    return 0;
}
