#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep

#define BUFFER_SIZE 5

typedef struct {
    int data;
} Item;

Item shared_buffer[BUFFER_SIZE];
int free_index = 0;
int full_index = 0;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

// Producer function
void* producer(void* arg) {
    while (1) {
        Item new_item;
        new_item.data = rand() % 100; // Produce an item
        usleep(100000);              // Simulate production delay (100ms)
        
        // Wait if buffer is full
        while ((free_index + 1) % BUFFER_SIZE == full_index) {
            usleep(50000); // Wait 50ms
        }

        pthread_mutex_lock(&buffer_mutex);
        shared_buffer[free_index] = new_item;
        free_index = (free_index + 1) % BUFFER_SIZE;
        printf("Produced: %d\n", new_item.data);
        pthread_mutex_unlock(&buffer_mutex);
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    while (1) {
        // Wait if buffer is empty
        while (free_index == full_index) {
            usleep(50000); // Wait 50ms
        }

        pthread_mutex_lock(&buffer_mutex);
        Item consumed_item = shared_buffer[full_index];
        full_index = (full_index + 1) % BUFFER_SIZE;
        printf("Consumed: %d\n", consumed_item.data);
        pthread_mutex_unlock(&buffer_mutex);

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

