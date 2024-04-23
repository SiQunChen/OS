#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

// Function prototypes
void *quickSort(void *params);
void *merge(void *params);

// Global variables
int arr[SIZE] = {12, 5, 3, 9, 8, 7, 6, 2, 1, 10};

typedef struct {
    int low;
    int high;
} parameters;

int main() {
    // Create two threads for sorting the two halves of the array
    pthread_t tid1, tid2, tid3;
    
    parameters *params1 = (parameters *)malloc(sizeof(parameters));
    parameters *params2 = (parameters *)malloc(sizeof(parameters));
    params1->low = 0;
    params1->high = SIZE / 2 - 1;
    params2->low = SIZE / 2;
    params2->high = SIZE - 1;

    pthread_create(&tid1, NULL, quickSort, params1);
    pthread_create(&tid2, NULL, quickSort, params2);

    // Wait for the two threads to finish
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Create a thread to merge the two sorted halves
    parameters *mergeParams = (parameters *)malloc(sizeof(parameters));
    mergeParams->low = 0;
    mergeParams->high = SIZE - 1;
    pthread_create(&tid3, NULL, merge, mergeParams);

    // Wait for the merge thread to finish
    pthread_join(tid3, NULL);

    // Print the sorted array
    printf("Sorted array: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(params1);
    free(params2);
    free(mergeParams);
    return 0;
}

// Function to perform Quick Sort
void *quickSort(void *params) {
    parameters *p = (parameters *)params;
    int low = p->low;
    int high = p->high;
    if (low < high) {
        int i = low, j = high + 1;
        int pivot = arr[low];
        
        do {
            do {
                i++;
            } while (arr[i] < pivot && i <= high);
            do {
                j--;
            } while (arr[j] > pivot);
            if (i < j) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        } while (i < j);
        
        int temp = arr[low];
        arr[low] = arr[j];
        arr[j] = temp;

        parameters *params1 = (parameters *)malloc(sizeof(parameters));
        parameters *params2 = (parameters *)malloc(sizeof(parameters));
        params1->low = low;
        params1->high = j - 1;
        params2->low = j + 1;
        params2->high = high;

        pthread_t tid1, tid2;
        pthread_create(&tid1, NULL, quickSort, params1);
        pthread_create(&tid2, NULL, quickSort, params2);

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

        free(params1);
        free(params2);
    }
    pthread_exit(NULL);
}

// Function to merge two sorted halves of the array
void *merge(void *params) {
    parameters *p = (parameters *)params;
    int low = p->low;
    int high = p->high;
    int mid = low + (high - low) / 2;

    int *temp = (int *)malloc((high - low + 1) * sizeof(int));
    int i = low, j = mid + 1, k = 0;

    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= high) {
        temp[k++] = arr[j++];
    }

    for (i = low; i <= high; i++) {
        arr[i] = temp[i - low];
    }

    free(temp);
    pthread_exit(NULL);
}