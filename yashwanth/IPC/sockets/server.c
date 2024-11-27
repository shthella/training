#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024] = {0};
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 3);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    read(client_fd, buffer, sizeof(buffer));
    printf("Server received: %s\n", buffer);
    send(client_fd, "Hello from server!", 18, 0);

    close(client_fd);
    close(server_fd);
    return 0;
}

