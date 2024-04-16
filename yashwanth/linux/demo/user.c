#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define DEVICE "/dev/file_op"

int main() {
    int fd;
    char read_buffer[1024];
    char write_buffer[] = "Hello, kernel!";

    // Open the device
    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device.");
        return errno;
    }

    // Write to the device
    write(fd, write_buffer, sizeof(write_buffer));

    // Read from the device
    read(fd, read_buffer, sizeof(read_buffer));
    printf("Received message from kernel: %s\n", read_buffer);
	memset(read_buffer, '\0', sizeof(read_buffer));
	printf("Second time\n");
    // Attempt to read from empty buffer
    read(fd, read_buffer, sizeof(read_buffer));
	printf("Empty buffer\n");
    // Close the device
    close(fd);

    return 0;
}

