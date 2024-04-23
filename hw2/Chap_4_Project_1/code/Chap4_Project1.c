#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS  9

// 數獨解的範例
int sudoku[9][9] = {
    {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

// 每個線程需要的參數
typedef struct {
    int row;
    int col;
} parameters;

void print_sudoku() {
    for (int i = 0; i < 9; i++) {
    	for (int j = 0; j < 9; j++) {
	    printf("%d ", sudoku[i][j]);
    	}
    	    printf("\n");
    }
}

// 檢查某一行是否有效
void* check_row(void* param) {
    parameters* params = (parameters*) param;
    int row = params->row;
    int validation[9] = {0};
    for (int i = 0; i < 9; i++) {
        int num = sudoku[row][i];
        if (num < 1 || num > 9 || validation[num - 1] == 1) {
            pthread_exit((void*) 0);
        } else {
            validation[num - 1] = 1;
        }
    }
    pthread_exit((void*) 1);
}

int main() {
    pthread_t threads[NUM_THREADS];

    // 創建9個線程來檢查每一行
    for (int i = 0; i < 9; i++) {
        parameters* data = (parameters*) malloc(sizeof(parameters));
        data->row = i;
        data->col = -1;
        pthread_create(&threads[i], NULL, check_row, data);
    }

    // 等待所有線程完成
    for (int i = 0; i < 9; i++) {
        void* result;
        pthread_join(threads[i], &result);
        long is_valid = (long) result;
        if (is_valid == 0) {
            print_sudoku();
            printf("The Sudoku solution is not valid!\n");
            return -1;
        }
    }
    print_sudoku();
    printf("The Sudoku solution is valid!\n");
    return 0;
}
