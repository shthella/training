#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "myfifo"

int main() {
    char msg[] = "Hello from FIFO!";
    char buf[100];

    // Create FIFO
    mkfifo(FIFO_NAME, 0666);

    if (fork() == 0) { // Child process
        int fd = open(FIFO_NAME, O_RDONLY);
        read(fd, buf, sizeof(buf));
        printf("Child received: %s\n", buf);
        close(fd);
    } else { // Parent process
        int fd = open(FIFO_NAME, O_WRONLY);
        write(fd, msg, strlen(msg) + 1);
        close(fd);
    }

    unlink(FIFO_NAME); // Remove FIFO after usage
    return 0;
}

