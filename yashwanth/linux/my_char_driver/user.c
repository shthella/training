#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

#define DEVICE_PATH "/dev/mychardev"
#define IOCTL_MAGIC 'k'
#define IOCTL_HELLO _IOR(IOCTL_MAGIC, 1, char*)

int main() {
    int fd;
    char buf[256];
    char ioctl_buf[256];

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device...");
        return errno;
    }

    printf("Reading from device...\n");
    int ret = read(fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("Failed to read the message from the device.");
    } else {
        printf("The received message is: [%s]\n", buf);
    }

    printf("Writing to the device...\n");
    strcpy(buf, "Hello from user!");
    write(fd, buf, strlen(buf));

    printf("Using IOCTL to get message from the kernel...\n");
    ioctl(fd, IOCTL_HELLO, ioctl_buf);
    printf("IOCTL returned: [%s]\n", ioctl_buf);

    close(fd);

    // Access sysfs attribute
    FILE *sysfs_fp = fopen("/sys/kernel/mychar_kobject/mychar_attr", "r");
    if (sysfs_fp == NULL) {
        perror("Failed to open sysfs entry...");
        return errno;
    }
    fgets(buf, sizeof(buf), sysfs_fp);
    printf("Sysfs entry content: %s\n", buf);
    fclose(sysfs_fp);

    return 0;
}

