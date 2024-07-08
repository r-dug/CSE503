#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;


int sum;
int array_size;
// function to add to the global sum
void *add_vector_to_sum(void *param){
	vector<int>& temp = *reinterpret_cast<vector<int>*>(param);
	for (int i = 0; i < temp.size(); i++)
		sum += temp[i];
	pthread_exit(0);
}

void *add_array_to_sum(void *param ){
	int *nums = (int *) param;
	for (int i = 0; i < array_size; i++){
		sum += nums[i];
	}
	pthread_exit(0);
}

vector<int> create_nums(string path){
	vector<int> nums;
	int num;

	ifstream file_in;
	file_in.open(path);

	while(file_in >> num)
		nums.push_back(num);
	
	file_in.close();
	return nums;

}

vector<int> slicing(vector<int>& arr,
                    int X, int Y)
{
 
    // Starting and Ending iterators
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y;
 
    // To store the sliced vector
    vector<int> result(Y - X );
 
    // Copy vector using copy function()
    copy(start, end, result.begin());
 
    // Return the final sliced vector
    return result;
}

int main(){
	auto start = high_resolution_clock::now();

	int NUM_THREADS = 10;	
	pthread_t tids[NUM_THREADS];
	pthread_attr_t attr[NUM_THREADS];
	
	string file_path = "input.txt";
	vector<int> nums = create_nums(file_path);
		
	vector<int> slice;
	
	for(int j = 0; j< NUM_THREADS; j++ ){
		slice = slicing(nums,j*(100000/NUM_THREADS), j*(100000/NUM_THREADS)+(100000/NUM_THREADS)); 
		pthread_attr_init(&attr[j]);
		pthread_create(&tids[j], &attr[j], add_vector_to_sum, &slice);
	}

	for(int j = 0; j < NUM_THREADS; j++){
		pthread_join(tids[j], NULL);
	}
	auto stop = high_resolution_clock::now();
	
	auto threaded_duration = duration_cast<microseconds>(stop - start);
	cout << "calculated sum (10 threaded): " << sum << endl;

	sum = 0;

	start = high_resolution_clock::now();
	NUM_THREADS = 1;

        for(int j = 0; j< NUM_THREADS; j++ ){
                slice = slicing(nums,j*(100000/NUM_THREADS), j*(100000/NUM_THREADS)+(100000/NUM_THREADS));
                pthread_attr_init(&attr[j]);
                pthread_create(&tids[j], &attr[j], add_vector_to_sum, &slice);
        }

        for(int j = 0; j < NUM_THREADS; j++){
                pthread_join(tids[j], NULL);
        }
        stop = high_resolution_clock::now();

        auto single_duration = duration_cast<microseconds>(stop - start);

	cout << "Calculated sum (1 thread): " << sum << endl;
	cout << "10 thread runtime: " << threaded_duration.count() << endl;
	cout << "1 thread runtime: " << single_duration.count() << endl; 

	// INTERESTING the above approach, using vectors, is slow.
	// I feel this might simply be caused by slicing the vector...
	// That does add an 10 iterations through the initial vector
	// below I am going to see if I can do better with an array solution
	int file_size = 0;
	ifstream fin;
	fin.open("input.txt");
	int num;
	while(fin >> num)
		file_size+=1;
	fin.close();
	
	//cout << "Lines in file: " << file_length << endl;
	
	NUM_THREADS = 10;
	int threaded_array_length = file_size/NUM_THREADS;
	int single_thread_array[file_size];
	int multithread_matrix[NUM_THREADS][threaded_array_length];
	

	//populate the arrays
	fin.open("input.txt");
	for (int i = 0; i < file_size; i++){
		fin >> single_thread_array[i];
	}
	fin.close();
	cout << "single threaded array created" << endl;

	for (int i = 0; i < NUM_THREADS; i++){
		for (int j = 0; j < threaded_array_length; j++){
			multithread_matrix[ i ][ j ] = single_thread_array[ i * threaded_array_length + j ];
		}
	}
	cout << "matrices created" << endl;

	//calculate sum using single thread
	sum = 0;
	array_size = file_size;
	start = high_resolution_clock::now();
	pthread_attr_init(&attr[0]);
        pthread_create(&tids[0], &attr[0], add_array_to_sum, &single_thread_array);
	
	stop = high_resolution_clock::now();
	auto single_thread_array_duration = duration_cast<microseconds>(stop-start);
	pthread_join(tids[0], NULL);
	cout << "Calculated sum (1 threadusing an aray): " << sum << endl;
        cout << "1 thread runtime: " << single_thread_array_duration.count() << endl;



	// calculate sum using 10 threads 
	sum = 0;
	array_size = file_size/NUM_THREADS;
	start = high_resolution_clock::now();
	for (int i = 0; i < NUM_THREADS; i++){
		pthread_attr_init(&attr[i]);
		pthread_create(&tids[i], &attr[i], add_array_to_sum, &multithread_matrix[i]);
	}
	
	for(int i = 0; i < NUM_THREADS; i++){
                pthread_join(tids[i], NULL);
        }
	stop = high_resolution_clock::now();
	auto multi_matrix_duration = duration_cast<microseconds>(stop-start);
	cout << "Calculated sum (10 threads on arrays): " << sum << endl;
        cout << "10 thread runtime: " << multi_matrix_duration.count() << endl;

	return 0;
}
