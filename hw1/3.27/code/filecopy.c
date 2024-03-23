#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 打開原檔案
    int source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 創建管道
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // 子進程
        close(pipefd[1]); // 關閉管道的寫端

        // 打開目標檔案，如果不存在則創建，並具有適當的權限
        int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (dest_fd == -1) {
            perror("open");
            close(pipefd[0]); // 關閉管道的讀端
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, BUFFER_SIZE)) > 0) {
            if (write(dest_fd, buffer, bytes_read) == -1) {
                perror("write");
                close(dest_fd);
                close(pipefd[0]);
                exit(EXIT_FAILURE);
            }
        }

        close(dest_fd);
        close(pipefd[0]); // 關閉管道的讀端
        exit(EXIT_SUCCESS);
    } else { // 父進程
        close(pipefd[0]); // 關閉管道的讀端

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
            if (write(pipefd[1], buffer, bytes_read) == -1) {
                perror("write");
                close(source_fd);
                close(pipefd[1]);
                exit(EXIT_FAILURE);
            }
        }

        close(source_fd);
        close(pipefd[1]); // 關閉管道的寫端
        wait(NULL); // 等待子進程完成
        exit(EXIT_SUCCESS);
    }

    return 0;
}
