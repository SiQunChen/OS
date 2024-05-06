#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

// 緩衝區
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// 信號量
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void *producer(void *arg) {
    int item;
    while (1) {
        // 生產一個項目
        item = rand() % 100;
        sem_wait(&empty); // 獲取一個空位
        pthread_mutex_lock(&mutex); // 進入臨界區
        buffer[in] = item;
        printf("Producer produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex); // 離開臨界區
        sem_post(&full); // 釋放一個已填充的項目
        sleep(rand() % 3); // 休眠一段時間
    }
}

void *consumer(void *arg) {
    int item;
    while (1) {
        sem_wait(&full); // 獲取一個已填充的項目
        pthread_mutex_lock(&mutex); // 進入臨界區
        item = buffer[out];
        printf("Consumer consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex); // 離開臨界區
        sem_post(&empty); // 釋放一個空位
        sleep(rand() % 3); // 休眠一段時間
    }
}

int main() {
    pthread_t prod_thread, cons_thread;
    sem_init(&empty, 0, BUFFER_SIZE); // 初始化 empty 信號量
    sem_init(&full, 0, 0); // 初始化 full 信號量
    pthread_mutex_init(&mutex, NULL); // 初始化互斥鎖

    pthread_create(&prod_thread, NULL, producer, NULL); // 建立生產者線程
    pthread_create(&cons_thread, NULL, consumer, NULL); // 建立消費者線程

    pthread_join(prod_thread, NULL); // 等待生產者線程結束
    pthread_join(cons_thread, NULL); // 等待消費者線程結束

    sem_destroy(&empty); // 銷毀信號量
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex); // 銷毀互斥鎖

    return 0;
}