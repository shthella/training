#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define DEVICE_PATH "/dev/mmap_driver"
#define MEM_SIZE 4096 // 4KB

void read_memory(uintptr_t addr) {
    int fd = open(DEVICE_PATH, O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("Failed to open device");
        return;
    }

    // Map the physical memory
    void *map_base = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr & ~(MEM_SIZE - 1));
    if (map_base == MAP_FAILED) {
        perror("Failed to mmap");
        close(fd);
        return;
    }

    uintptr_t offset = addr & (MEM_SIZE - 1);
    uint32_t value = *((uint32_t *)(map_base + offset));
    printf("Value at address 0x%lx: 0x%x\n", addr, value);

    munmap(map_base, MEM_SIZE);
    close(fd);
}

void write_memory(uintptr_t addr, uint32_t value) {
    int fd = open(DEVICE_PATH, O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("Failed to open device");
        return;
    }

    // Map the physical memory
    void *map_base = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr & ~(MEM_SIZE - 1));
    if (map_base == MAP_FAILED) {
        perror("Failed to mmap");
        close(fd);
        return;
    }

    uintptr_t offset = addr & (MEM_SIZE - 1);
    *((uint32_t *)(map_base + offset)) = value;
    printf("Wrote 0x%x to address 0x%lx\n", value, addr);

    munmap(map_base, MEM_SIZE);
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <read|write> <address> [value]\n", argv[0]);
        return -1;
    }

    uintptr_t address = strtoul(argv[2], NULL, 0);

    if (strcmp(argv[1], "read") == 0) {
        read_memory(address);
    } else if (strcmp(argv[1], "write") == 0 && argc == 4) {
        uint32_t value = strtoul(argv[3], NULL, 0);
        write_memory(address, value);
    } else {
        printf("Invalid arguments\n");
        return -1;
    }

    return 0;
}

