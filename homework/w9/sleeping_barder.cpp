#include <iostream>
#include <thread>
#include <semaphore.h>
#include <vector>
#include <queue>
#include <unistd.h>
#include <mutex>
using namespace std;
// Define the number of chairs in the waiting room
const int CHAIRS = 5;
queue<int> customers_waiting;

// Semaphores and mutex
sem_t customers;
sem_t barber;
sem_t accessSeats;
mutex printMutex;
int freeSeats = CHAIRS;
int current;

// function to pretty print the seats
void print_seats(queue<int> customers){
    int customer;
    cout << "waiting room: {";
    for (int i = 0; i < CHAIRS;  i++){
        if (i<customers.size()){
            customer = customers.front();
            cout << " | " << customer;
            customers.pop();
            customers.push(customer);
        }else{
            cout << " | " << '_';
        }
    }
    cout << " | }" << endl;
}
// Barber function
void barber_function() {
    while (true) {
	    // Wait for a customer to arrive
        sem_wait(&customers);
        // Lock the access to the seat count
        sem_wait(&accessSeats);
        // A customer is getting a haircut, so a seat is freed
        freeSeats++;
        current = customers_waiting.front();
        customers_waiting.pop();
        // Signal that the barber is ready
        sem_post(&barber);
        // Unlock the access to the seat count
        sem_post(&accessSeats);
        // The barber is cutting hair (simulate with sleep)
        printMutex.lock();
        system("clear");
        
        cout << "Barber: cutting hair of " << current << endl;
        sleep(5);  // Simulate time taken to cut hair
        printMutex.unlock();
    }
}

// Customer function
void customer_function(int id) {
    while (true) {
        // Lock the access to the seat count
        sem_wait(&accessSeats);

        if (freeSeats > 0) {
            print_seats(customers_waiting);
            // There is a free seat, take it
        	freeSeats--;
		    customers_waiting.push(id);
            sleep(1);
            // Signal that there is a new customer
            sem_post(&customers);
            // Unlock the access to the seat count
            sem_post(&accessSeats);
            // Wait for the barber to be ready
            sem_wait(&barber);
        } else {
            // No free seats, leave the barbershop
            sem_post(&accessSeats);
            cout << "Customer " << id << " is leaving, no free seats" << endl;
    }
    // Simulate time between customer visits
    sleep(1);
    }
}

int main() {
    // Initialize the semaphores
    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);
    sem_init(&accessSeats, 0, 1);

    // Create the barber thread
    std::thread barberThread(barber_function);

    // Create customer threads. the amount of threads is actually arbitrary, but if you have fewer than the seats available, the seats will never fill.
    std::vector<std::thread> customerThreads;
    for (int i = 1; i < 8; ++i) {
        customerThreads.push_back(thread(customer_function, i));
    }

    // Join customer threads (in this example, they run indefinitely)
    for (auto& t : customerThreads) {
        t.join();
    }

    // Join barber thread (in this example, it runs indefinitely)
    barberThread.join();

    // Destroy the semaphores
    sem_destroy(&customers);
    sem_destroy(&barber);

    return 0;
}

