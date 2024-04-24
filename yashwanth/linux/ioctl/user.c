#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_NAME "/dev/mychardev"
#define IOCTL_SET_MSG _IOW(0, 0, char *)

int main() {
    int fd;
    char message[256] = "New message from user space";

    // Open the device file
    fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    // Write message to device
    printf("Writing message to device: %s\n", message);
    if (write(fd, message, sizeof(message)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return -1;
    }

    // Read message from device
    char read_buf[256];
    printf("Reading message from device:\n");
    if (read(fd, read_buf, sizeof(read_buf)) < 0) {
        perror("Failed to read from the device");
        close(fd);
        return -1;
    }
    printf("%s\n", read_buf);

    // Use IOCTL to set message
    char ioctl_message[256] = "New message set via IOCTL";
    printf("Setting message via IOCTL: %s\n", ioctl_message);
    if (ioctl(fd, IOCTL_SET_MSG, ioctl_message) < 0) {
        perror("IOCTL failed");
        close(fd);
        return -1;
    }

    // Close device file
    close(fd);

    return 0;
}

