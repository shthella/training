#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/mychardev"

int main() {
    int file_desc;
    int bytes_read;
    char buffer[BUFSIZ];

    // Open the device file
    file_desc = open(DEVICE, O_RDWR);
    if (file_desc < 0) {
        printf("Can't open device file: %s\n", DEVICE);
        return -1;
    }

    // Writing to the device
    printf("Writing to the device...\n");
    if (write(file_desc, "Hello from user space!", 22) < 0) {
        printf("Failed to write to the device\n");
        return -1;
    }

    // Reading from the device
    printf("Reading from the device...\n");
    bytes_read = read(file_desc, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        printf("Failed to read from the device\n");
        return -1;
    }

    // Null terminate the buffer
    buffer[bytes_read] = '\0';

    // Print the data read from the device
    printf("Data read from the device: %s\n", buffer);

    // Close the device file
    close(file_desc);

    return 0;
}

