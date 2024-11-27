#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>

#define QUEUE_NAME "/myqueue"

int main() {
    mqd_t mq;
    struct mq_attr attr = {0, 10, 256, 0};
    char msg[] = "Hello from message queue!";
    char buf[256];

    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);

    if (fork() == 0) { // Child process
        mq_receive(mq, buf, 256, NULL);
        printf("Child received: %s\n", buf);
    } else { // Parent process
        mq_send(mq, msg, strlen(msg) + 1, 0);
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME); // Clean up
    return 0;
}

