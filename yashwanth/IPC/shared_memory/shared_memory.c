#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/myshm"
#define SIZE 256

int main() {
    int shm_fd;
    char *shared_mem;

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644);
    ftruncate(shm_fd, SIZE);
    shared_mem = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (fork() == 0) { // Child process
        sleep(1); // Ensure parent writes first
        printf("Child received: %s\n", shared_mem);
    } else { // Parent process
        strcpy(shared_mem, "Hello from shared memory!");
        sleep(2); // Allow child to read
    }

    munmap(shared_mem, SIZE);
    shm_unlink(SHM_NAME); // Clean up
    return 0;
}

