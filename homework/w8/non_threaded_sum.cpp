#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(){
	auto start = high_resolution_clock::now();

	ifstream fin;
	fin.open("input.txt");
	int sum;

	int num; 

	while(fin >> num){
		sum += num;
	}

	auto stop = high_resolution_clock::now();
	
	auto duration = duration_cast<microseconds>(stop - start);

	cout << "Sum: " << sum << endl;

	cout << "Runtime: " << duration.count() << endl;

	return 0;
}
