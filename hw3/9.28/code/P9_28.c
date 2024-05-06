#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <virtual address>\n", argv[0]);
        return 1;
    }

    uint32_t virtual_address = strtoul(argv[1], NULL, 10);
    uint32_t page_size = 4096; // 4KB page size
    uint32_t page_mask = 0xFFFFF000; // 用於清除偏移量位

    uint32_t page_number = (virtual_address & page_mask) >> 12; // 頁號 = (虛擬地址 & 頁面遮罩) >> 12
    uint32_t offset = virtual_address & ~page_mask; // 偏移量 = 虛擬地址 & ~頁面遮罩

    printf("The address %u contains:\n", virtual_address);
    printf("page number=%u\n", page_number);
    printf("offset=%u\n", offset);

    return 0;
}