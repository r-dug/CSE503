#include <forward_list>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

using namespace std;

int* tokenizer(string &s, char del) {
    stringstream ss(s);
    string detail;
    int* job_list = new int[3];
    int i = 0;
    while (getline(ss, detail, del) && i < 3) {
        job_list[i] = stoi(detail);
        i++;
    }
    return job_list;
}

class Job {
private:
    int job_id;
    int time_in;
    int duration;
    Job* next;

public:
    // Constructors and destructor
    Job() : job_id(0), time_in(0), duration(0), next(nullptr) {}
    Job(int job_id, int time_in, int duration) 
        : job_id(job_id), time_in(time_in), duration(duration), next(nullptr) {}

    // Accessors
    int getId() const { return job_id; }
    int getTimeIn() const { return time_in; }
    int getDuration() const { return duration; }
    Job* getNext() const { return next; }

    // Mutators
    void setId(int id) { this->job_id = id; }
    void setTimeIn(int time_in) { this->time_in = time_in; }
    void setDuration(int duration) { this->duration = duration; }
    void setNext(Job* next) { this->next = next; }

    void reduceDuration(int time_q) { duration -= time_q; }
};

class JobsQueue {
private:
    Job* front;
    Job* rear;
    int size;
    const int quantum = 5;

public:
    // Constructor
    JobsQueue() : front(nullptr), rear(nullptr), size(0) {}

    // Destructor
    ~JobsQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Accessors
    bool isEmpty() const { return front == nullptr; }
    int getTimeQ() const { return quantum; }
    Job* getFront() const { return front; }

    void printRunningJob() const {
        if (front->getDuration() > quantum) {
            cout << "Job " << front->getId() << ", scheduled for " << quantum << "ms" << endl;
            this_thread::sleep_for(chrono::milliseconds(quantum));
        } else {
            cout << "Job " << front->getId() << ", scheduled for " << front->getDuration() << "ms | COMPLETED" << endl;
            this_thread::sleep_for(chrono::milliseconds(front->getDuration()));
        }
    }

    // Mutators
    void enqueue(Job* new_job) {
        if (isEmpty()) {
            front = rear = new_job;
        } else {
            rear->setNext(new_job);
            rear = new_job;
        }
        size++;
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty, cannot dequeue." << endl;
            return;
        }
        Job* temp = front;
        front = front->getNext();
        delete temp;
        size--;
    }
};

int the_time = 0;

int main() {

    JobsQueue jobs_queue;

    // Populate queue with input from text file
    string file_path = "input.txt";
    ifstream file_in(file_path);

    if (!file_in.is_open()) {
        cerr << "Error opening file: " << file_path << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    forward_list<string> jobsList;

    while (file_in >> line) {
        jobsList.push_front(line);
    }
    file_in.close();

    for (string job_line : jobsList) {
        int* job_list = tokenizer(job_line, ',');
        Job* new_job = new Job(job_list[0], job_list[1], job_list[2]);
        jobs_queue.enqueue(new_job);
        delete[] job_list;
    }

    while (!jobs_queue.isEmpty()) {
        if (jobs_queue.getFront()->getTimeIn() > the_time) {
            the_time++;
            continue;
        }

        jobs_queue.printRunningJob();
        jobs_queue.getFront()->reduceDuration(jobs_queue.getTimeQ());

        int curr_job_duration = jobs_queue.getFront()->getDuration();
        int curr_job_id = jobs_queue.getFront()->getId();
        int curr_job_time_in = jobs_queue.getFront()->getTimeIn();

        if (jobs_queue.getFront()->getDuration() >= jobs_queue.getTimeQ()) {
            the_time += jobs_queue.getTimeQ();
        } else {
            the_time += jobs_queue.getFront()->getDuration();
        }
        
        if (curr_job_duration > 0) {
            Job* temp = new Job(curr_job_id, curr_job_time_in, curr_job_duration);
            jobs_queue.enqueue(temp);
        }

        jobs_queue.dequeue();
    }

    return 0;
}
