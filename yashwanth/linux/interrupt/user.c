#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/my_device", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Simulate interrupt
    write(fd, NULL, 0);  // This depends on how your driver handles this

    close(fd);
    return 0;
}

