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

#include <stdio>
#include <stdlib>

using namespace std;

class Job{
	private:
		int job_id;
		int time_in;
		int duration;
	public:
		// constructors and destructor
		Job(){	};
		Job(int job_id, int time_in, int duration){
			job_id = job_id;
			time_in = time_in;
			duration = duration;
		};
		~Job(){};

		// Accessors
		int get_id(){
			return job_id;
		}
		int get_time_in(){
			return time_in;
		};
		int get_duration(){
			return duration;
		};


		// Mutators
		void reduce_duration(int time_q){
			duration -= time_q;
		};
		//void
}


template <class T, class Container = deque<T>
class 
{
	private:
		int size;
				
};
