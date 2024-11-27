#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

sem_t sem;

void *producer(void *arg) {
    printf("Producer waiting...\n");
    sem_wait(&sem);
    printf("Producer working...\n");
    sem_post(&sem);
    return NULL;
}

void *consumer(void *arg) {
    printf("Consumer working...\n");
    sem_post(&sem);
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&sem, 0, 0);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&sem);
    return 0;
}

