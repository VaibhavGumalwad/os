5) dinning philosopher
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // Number of philosophers

sem_t chopsticks[N];
sem_t mutex; // Mutex to protect critical sections

void *philosopher(void *arg) {
    int id = *((int *)arg);
    printf("Philosopher %d is thinking.\n", id);

    while (1) {
        // Wait for the left chopstick
        sem_wait(&chopsticks[id]);
        printf("Philosopher %d picked up left chopstick.\n", id);

        // Wait for the right chopstick
        sem_wait(&chopsticks[(id + 1) % N]);
        printf("Philosopher %d picked up right chopstick.\n", id);

        // Eat
        printf("Philosopher %d is eating.\n", id);

        // Release the chopsticks
        sem_post(&chopsticks[id]);
        sem_post(&chopsticks[(id + 1) % N]);

        // Think
        printf("Philosopher %d is thinking.\n", id);

        // Introduce a small delay to make it easier to observe the interleaving
        usleep(100000);
    }
}

int main() {
    pthread_t philosophers[N];
    int ids[N];

    // Initialize semaphores
    for (int i = 0; i < N; i++) {
        sem_init(&chopsticks[i], 0, 1);
    }
    sem_init(&mutex, 0, 1);

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for the threads to finish
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < N; i++) {
        sem_destroy(&chopsticks[i]);
    }
    sem_destroy(&mutex);

    return 0;
}
