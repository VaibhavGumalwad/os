4) READER WRITER

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define READERS_COUNT 3
#define WRITERS_COUNT 2

int data = 0;
int readers_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t readers_sem, writers_sem;

void *reader(void *arg) {
    int reader_id = *(int *)arg;
    free(arg);

    while (1) {
        sleep(1);  // Simulate reading activity

        sem_wait(&readers_sem);
        readers_count++;
        if (readers_count == 1) {
            sem_wait(&writers_sem);  // Prevent writers when the first reader arrives
        }
        sem_post(&readers_sem);

        printf("Reader %d is reading data: %d\n", reader_id, data);

        sem_wait(&readers_sem);
        readers_count--;
        if (readers_count == 0) {
            sem_post(&writers_sem);  // Allow writers when the last reader leaves
        }
        sem_post(&readers_sem);
    }
}

void *writer(void *arg) {
    int writer_id = *(int *)arg;
    free(arg);

    while (1) {
        sleep(2);  // Simulate writing activity

        sem_wait(&writers_sem);
        pthread_mutex_lock(&mutex);  // Acquire mutual exclusion for writing
        data++;
        printf("Writer %d is writing data: %d\n", writer_id, data);
        pthread_mutex_unlock(&mutex);  // Release mutual exclusion for writing
        sem_post(&writers_sem);
    }
}

int main() {
    sem_init(&readers_sem, 0, 1);
    sem_init(&writers_sem, 0, 1);

    pthread_t readers[READERS_COUNT];
    pthread_t writers[WRITERS_COUNT];

    for (int i = 0; i < READERS_COUNT; i++) {
        int *reader_id = malloc(sizeof(int));
        *reader_id = i;
        pthread_create(&readers[i], NULL, reader, reader_id);
    }

    for (int i = 0; i < WRITERS_COUNT; i++) {
        int *writer_id = malloc(sizeof(int));
        *writer_id = i;
        pthread_create(&writers[i], NULL, writer, writer_id);
    }

    for (int i = 0; i < READERS_COUNT; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < WRITERS_COUNT; i++) {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&readers_sem);
    sem_destroy(&writers_sem);

    return 0;
}
