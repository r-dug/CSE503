#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <vector>
#include <unistd.h>

#define CHAIRS 5 // Number of chairs in the waiting room

sem_t customers;   // To track the number of waiting customers
sem_t barberReady; // To signal the barber's readiness
sem_t accessSeats; // Mutex to protect access to seat count
int freeSeats = CHAIRS; // Number of free seats in the waiting room

std::mutex printMutex; // Mutex to protect printing to stdout

void barber_function() {
    while (true) {
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
        std::lock_guard<std::mutex> lock(printMutex);
        std::cout << "The barber is cutting hair." << std::endl;

        // The barber is cutting hair (simulate with sleep)
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate time taken to cut hair
    }
}

void customer_function(int id) {
    while (true) {
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
            {
                std::lock_guard<std::mutex> lock(printMutex);
                std::cout << "Customer " << id << " is waiting." << std::endl;
            }

            // Wait for the barber to be ready
            sem_wait(&barberReady);

            // Critical section for printing
            {
                std::lock_guard<std::mutex> lock(printMutex);
                std::cout << "Customer " << id << " is getting a haircut." << std::endl;
            }

            // Simulate getting a haircut
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate time taken to get a haircut
        } else {
            // No free seats, leave the barbershop
            sem_post(&accessSeats);

            // Critical section for printing
            {
                std::lock_guard<std::mutex> lock(printMutex);
                std::cout << "Customer " << id << " is leaving, no free seats." << std::endl;
            }
        }
        // Simulate time between customer visits
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 5 + 1));
    }
}

int main() {
    // Initialize the semaphores
    sem_init(&customers, 0, 0);
    sem_init(&barberReady, 0, 0);
    sem_init(&accessSeats, 0, 1);

    // Create the barber thread
    std::thread barberThread(barber_function);

    // Create customer threads
    std::vector<std::thread> customerThreads;
    for (int i = 0; i < 10; ++i) {
        customerThreads.emplace_back(customer_function, i + 1);
    }

    // Join customer threads (they run indefinitely in this example)
    for (auto& t : customerThreads) {
        t.join();
    }

    // Join barber thread (it runs indefinitely in this example)
    barberThread.join();

    // Destroy the semaphores
    sem_destroy(&customers);
    sem_destroy(&barberReady);
    sem_destroy(&accessSeats);

    return 0;
}

