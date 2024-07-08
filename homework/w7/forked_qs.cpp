#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <signal.h>
using namespace std;


int main()
{
	// initialize variables
	int query;
	int count;
	int rand_ints[1500];
	int root_pid;

	// get process ID for current process
	root_pid = getpid();

	// create array of random ints
	for (int i = 0; i < 1500; i++)
		rand_ints[i] = rand() % 120;

	// create file reader opject and open the text file of mqueries
	ifstream fin;
	fin.open("input.txt");

	// iterate through all queries
	while(fin >> query)
	{
		//check that PID before fork is not same as after fork
		int old_pid = getpid();
		fork();
		// this was how I handled creating new processes without repeating work...
		if (old_pid == getpid())
		{
			return -1;
		}
		// iterate through array, counting the occorrences of the query value
		for (int i = 0; i < 1200; i++)
			if (query == rand_ints[i])
				count++;
		cout << "query: " << query << "\t count:" << count << "\t PID:" << getpid() << endl;
	}
	// kill processes, cleaning up
	kill(root_pid, 0);
	return 0;
}
