#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep

#define QUEUE_SIZE 10

typedef struct {
    int data;
} Message;

Message message_queue[QUEUE_SIZE];
int queue_front = 0, queue_rear = 0, queue_count = 0;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_not_full = PTHREAD_COND_INITIALIZER;

// Function to add a message to the queue
void enqueue(Message msg) {
    message_queue[queue_rear] = msg;
    queue_rear = (queue_rear + 1) % QUEUE_SIZE;
    queue_count++;
}

// Function to remove a message from the queue
Message dequeue() {
    Message msg = message_queue[queue_front];
    queue_front = (queue_front + 1) % QUEUE_SIZE;
    queue_count--;
    return msg;
}

// Producer function
void* producer(void* arg) {
    while (1) {
        Message msg;
        msg.data = rand() % 100; // Produce a random message
        usleep(100000);          // Simulate production delay (100ms)

        pthread_mutex_lock(&queue_mutex);
        while (queue_count == QUEUE_SIZE) {
            // Wait if the queue is full
            pthread_cond_wait(&queue_not_full, &queue_mutex);
        }

        enqueue(msg);
        printf("Produced: %d\n", msg.data);

        pthread_cond_signal(&queue_not_empty); // Notify a waiting consumer
        pthread_mutex_unlock(&queue_mutex);
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&queue_mutex);
        while (queue_count == 0) {
            // Wait if the queue is empty
            pthread_cond_wait(&queue_not_empty, &queue_mutex);
        }

        Message msg = dequeue();
        printf("Consumed: %d\n", msg.data);

        pthread_cond_signal(&queue_not_full); // Notify a waiting producer
        pthread_mutex_unlock(&queue_mutex);

        usleep(100000); // Simulate consumption delay (100ms)
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish (they won't in this case)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}

