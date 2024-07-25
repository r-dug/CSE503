#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CHAIRS 5 // Number of chairs in the waiting room

sem_t customers;   // To track the number of waiting customers
sem_t barberReady; // To signal the barber's readiness
sem_t accessSeats; // Mutex to protect access to seat count
int freeSeats = CHAIRS; // Number of free seats in the waiting room

pthread_mutex_t printMutex; // Mutex to protect printing to stdout

void* barber_function(void* arg) {
    while (1) {
        // Wait for a customer to arrive
        sem_wait(&customers);
        // Lock the access to the seat count
        sem_wait(&accessSeats);
        // A customer is ready for a haircut, so a seat is freed
        freeSeats++;
        // Signal that the barber is ready to cut hair
        sem_post(&barberReady);
        // Unlock the access to the seat count
        sem_post(&accessSeats);

        // Critical section for printing
        pthread_mutex_lock(&printMutex);
        printf("The barber is cutting hair.\n");
        pthread_mutex_unlock(&printMutex);

        // The barber is cutting hair (simulate with sleep)
        sleep(2); // Simulate time taken to cut hair
    }
    return NULL;
}

void* customer_function(void* arg) {
    int id = *((int*)arg);
    while (1) {
        // Lock the access to the seat count
        sem_wait(&accessSeats);
        if (freeSeats > 0) {
            // There is a free seat, take it
            freeSeats--;
            // Signal that there is a new customer
            sem_post(&customers);
            // Unlock the access to the seat count
            sem_post(&accessSeats);

            // Critical section for printing
            pthread_mutex_lock(&printMutex);
            printf("Customer %d is waiting.\n", id);
            pthread_mutex_unlock(&printMutex);

            // Wait for the barber to be ready
            sem_wait(&barberReady);

            // Critical section for printing
            pthread_mutex_lock(&printMutex);
            printf("Customer %d is getting a haircut.\n", id);
            pthread_mutex_unlock(&printMutex);

            // Simulate getting a haircut
            sleep(1); // Simulate time taken to get a haircut
        } else {
            // No free seats, leave the barbershop
            sem_post(&accessSeats);

            // Critical section for printing
            pthread_mutex_lock(&printMutex);
            printf("Customer %d is leaving, no free seats.\n", id);
            pthread_mutex_unlock(&printMutex);
        }
        // Simulate time between customer visits
        sleep(rand() % 5 + 1);
    }
    return NULL;
}

int main() {
    // Initialize the semaphores
    sem_init(&customers, 0, 0);
    sem_init(&barberReady, 0, 0);
    sem_init(&accessSeats, 0, 1);

    // Initialize the mutex for printing
    pthread_mutex_init(&printMutex, NULL);

    pthread_t barberThread;
    pthread_create(&barberThread, NULL, barber_function, NULL);

    pthread_t customerThreads[10];
    int customerIds[10];
    for (int i = 0; i < 4; ++i) {
        customerIds[i] = i + 1;
        pthread_create(&customerThreads[i], NULL, customer_function, &customerIds[i]);
    }

    for (int i = 0; i < 10; ++i) {
        pthread_join(customerThreads[i], NULL);
    }

    pthread_join(barberThread, NULL);

    // Destroy the semaphores and mutex
    sem_destroy(&customers);
    sem_destroy(&barberReady);
    sem_destroy(&accessSeats);
    pthread_mutex_destroy(&printMutex);

    return 0;
}

