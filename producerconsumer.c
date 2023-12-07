4)producer consumer
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

// Shared buffer
int buffer[BUFFER_SIZE];

// Index for insertion and removal in the buffer
int in = 0;
int out = 0;

// Mutex to protect the critical section (buffer access)
pthread_mutex_t mutex;

// Counting semaphores
sem_t full;
sem_t empty;

// Number of producer and consumer threads
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

// Number of items each producer and consumer produces/consumes
#define ITEMS_PER_PRODUCER 3
#define ITEMS_PER_CONSUMER 2

// Function prototypes
void* producer(void* arg);
void* consumer(void* arg);
void insertItem(int item);
int removeItem();

int main() {
    // Initialize mutex and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);

    // Thread identifiers
    pthread_t producer_threads[NUM_PRODUCERS];
    pthread_t consumer_threads[NUM_CONSUMERS];

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producer_threads[i], NULL, producer, NULL);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumer_threads[i], NULL, consumer, NULL);
    }

    // Join producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Destroy mutex and semaphores
    pthread_mutex_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}

void* producer(void* arg) {
    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        // Produce item
        int item = rand() % 100;
        
        // Wait for an empty slot in the buffer
        sem_wait(&empty);

        // Enter critical section (buffer access)
        pthread_mutex_lock(&mutex);

        // Insert item into the buffer
        insertItem(item);
        printf("Produced item %d\n", item);

        // Exit critical section
        pthread_mutex_unlock(&mutex);

        // Signal that the buffer is no longer empty
        sem_post(&full);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    for (int i = 0; i < ITEMS_PER_CONSUMER; i++) {
        // Wait for a full slot in the buffer
        sem_wait(&full);

        // Enter critical section (buffer access)
        pthread_mutex_lock(&mutex);

        // Remove item from the buffer
        int item = removeItem();
        printf("Consumed item %d\n", item);

        // Exit critical section
        pthread_mutex_unlock(&mutex);

        // Signal that the buffer is no longer full
        sem_post(&empty);
    }

    pthread_exit(NULL);
}

void insertItem(int item) {
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
}

int removeItem() {
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    return item;
}
