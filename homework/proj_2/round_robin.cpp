/* 
Title: Round Robin Scheduling | CSE 503

Author: Richard Douglas

Description:
This program is designed to simulate a round robin task scheduler. 
it is a simplistic version, implementing the basic logic underlyng 
this type of task scheduler. 

A FIFO ready queue is initialized, and as processes come in, they are allowed 
to execute for a specified amount of time (time quantum), at which 
point the scheduler moves to the next task is executed for a time 
quantum.If the process is not complete, or otherwise is no longer in 
a "ready" state, it is simply dequeued. Otherwise, the task is 
enqueued. This process is repeated until all of the jobs are completed
(or indefinitely while an OS is running in a real world situation). 


*/
#include <forward_list>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

int* tokenizer(string &s, char del)
{
    stringstream ss(s);
    string detail;
    int* job_list = new int[3];
    int i = 0;
    while (!ss.eof()) {
        getline(ss, detail, del);
        job_list[i] = stoi(detail);
    }
    return job_list;
}


class Job{
	private:
		int job_id;
		int time_in;
		int duration;
        Job* next = nullptr;
	public:
		// constructors and destructor
		Job(){	};
		Job(int job_id, int time_in, int duration){
			this->job_id = job_id;
			this->time_in = time_in;
			this->duration = duration;
		};
		~Job(){};

		// Accessors
		int getId() const {
			return job_id;
		}
		int getTimeIn() const {
			return time_in;
		};
		int getDuration() const {
			return duration;
		};
        Job* getNext() const {
            return next;
        }

		// Mutators
        void setId(int id){
			this->job_id = id;
		}
		void setTimeIn(int time_in){
			this->time_in = time_in;
		};
		void setDuration(int duration){
			this->duration = duration;
		};
        void setNext(Job* next){
            this->next = next;
        }

		void reduceDuration(int time_q){
			duration -= time_q;
		};
};

class JobsQueue {
private:
    Job* front;
    Job* rear;
    int size;
    const int quantum = 5;

public:
    //constructor
    JobsQueue() : front(nullptr), rear(nullptr), size(0) {}
    // Destructor
    ~JobsQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // accessors
    bool isEmpty() const {
        return front == nullptr;
    }

    int getTimeQ() const {
        return quantum;
    }
    Job* getFront() const {
        return front;
    }

    void printRunningJob() const {

        if (front->getDuration() >= quantum){
            cout << "Job " << front->getId() << ", scheduled for " << quantum << "ms" << endl;
        }
        else{
            cout << "Job " << front->getId() << ", scheduled for " << front->getDuration() << "ms" << endl;
        }
    }
    
    // mutators
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
            std::cout << "Queue is empty, cannot dequeue." << std::endl;
            return;
        }
        Job* temp = front;
        if (front->getNext())
            front = front->getNext();
        delete temp;
        size--;
    }
};

int the_time = 0;
int main(){
    
    JobsQueue jobs_queue = JobsQueue();

    //populate queue with input from text file
    string file_path = "input.txt";
    ifstream file_in(file_path);

    string line;
    forward_list<string> jobsList;
    
    if (!file_in.is_open()) {
        cerr << "Error opening file: " << file_path << endl;
        exit(EXIT_FAILURE);
    }

    while (file_in >> line)
        jobsList.push_front(line);
    file_in.close();

    for (string job_line : jobsList){
        int* job_list = tokenizer(job_line, ',');
        Job* new_job = new Job();
        new_job->setId(job_list[0]);
        new_job->setTimeIn(job_list[1]);
        new_job->setDuration(job_line[2]);
        jobs_queue.enqueue(new_job);
    }
    int curr_job_duration;  
    int curr_job_id;
    int curr_job_time_in;
    Job* temp = new Job;
    while (jobs_queue.isEmpty() != true){
        if (jobs_queue.getFront()->getTimeIn()<the_time) continue;
        curr_job_duration = jobs_queue.getFront()->getDuration();
        jobs_queue.printRunningJob();
        curr_job_duration -= jobs_queue.getTimeQ();
        curr_job_id = jobs_queue.getFront()->getId();
        curr_job_time_in = jobs_queue.getFront()->getTimeIn();
        temp->setId(curr_job_id);
        temp->setTimeIn(curr_job_time_in);
        temp->setDuration(curr_job_duration);
        
        jobs_queue.dequeue();
        if (curr_job_duration > 0)
            jobs_queue.enqueue(temp);
    }
}