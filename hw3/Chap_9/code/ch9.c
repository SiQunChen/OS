#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_MEMORY_SIZE 1000

typedef struct {
    int start_address;
    int end_address;
    bool allocated;
} MemoryBlock;

MemoryBlock memory[MAX_MEMORY_SIZE];

void initializeMemory(int total_memory) {
    memory[0].start_address = 0;
    memory[0].end_address = total_memory - 1;
    memory[0].allocated = false;
}

void requestMemory(int requested_size) {
    int i;
    for (i = 0; i < MAX_MEMORY_SIZE; i++) {
        if (!memory[i].allocated && (memory[i].end_address - memory[i].start_address + 1) >= requested_size) {
            // Allocate memory
            memory[i].allocated = true;
            memory[i].end_address = memory[i].start_address + requested_size - 1;
            printf("Memory block allocated from %d to %d\n", memory[i].start_address, memory[i].end_address);
            break;
        }
    }
    if (i == MAX_MEMORY_SIZE) {
        printf("Unable to allocate memory.\n");
    }
}

void releaseMemory(int start_address) {
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        if (memory[i].start_address == start_address && memory[i].allocated) {
            memory[i].allocated = false;
            printf("Memory block released from %d to %d\n", memory[i].start_address, memory[i].end_address);
            break;
        }
    }
}

void compactMemory() {
    int j = 0;
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        if (!memory[i].allocated) {
            continue;
        }
        if (i != j) {
            memory[j].start_address = memory[i].start_address;
            memory[j].end_address = memory[i].end_address;
            memory[j].allocated = true;
            memory[i].allocated = false;
        }
        j++;
    }
    printf("Memory compacted successfully.\n");
}

void reportMemory() {
    printf("Memory Report:\n");
    for (int i = 0; i < MAX_MEMORY_SIZE; i++) {
        if (!memory[i].allocated) {
            printf("Free block: %d to %d\n", memory[i].start_address, memory[i].end_address);
        } else {
            printf("Allocated block: %d to %d\n", memory[i].start_address, memory[i].end_address);
        }
    }
}

int main() {
    initializeMemory(MAX_MEMORY_SIZE);
    requestMemory(100);
    requestMemory(200);
    requestMemory(150);
    compactMemory();
    releaseMemory(0);
    reportMemory();
    return 0;
}
