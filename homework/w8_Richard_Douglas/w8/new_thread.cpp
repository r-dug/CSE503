#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct VectorThreadData {
    vector<int> nums;
    int local_sum;
};
struct ArrayThreadData {
    int array_size;
    int* nums;
    int local_sum;
};

void *add_vector_to_local_sum(void *param) {
    VectorThreadData *data = reinterpret_cast<VectorThreadData*>(param);
    data->local_sum = 0;
    for (int val : data->nums) {
        data->local_sum += val;
    }
    pthread_exit(0);
}

void *add_array_to_sum(void *param) {
    ArrayThreadData *data = reinterpret_cast<ArrayThreadData*>(param);
    data->local_sum = 0;
    int* nums_p = data->nums;
    for (int i = 0; i < data->array_size; i++) {
        try{
            data->local_sum += *nums_p;
            nums_p++;
        }
        catch(int exit){
            cout << "Fatal error:" << exit;
            break;
        }
    }
    pthread_exit(0);
}

vector<int> create_nums(const string &path) {
    vector<int> nums;
    int num;
    ifstream file_in(path);

    if (!file_in.is_open()) {
        cerr << "Error opening file: " << path << endl;
        exit(EXIT_FAILURE);
    }

    while (file_in >> num)
        nums.push_back(num);

    file_in.close();
    return nums;
}

vector<int> slicing(const vector<int> &arr, int start, int end) {
    return vector<int>(arr.begin() + start, arr.begin() + end);
}

int main() {
    string file_path = "input.txt";

    vector<int> nums = create_nums(file_path);

    int file_size = nums.size();
    int NUM_THREADS = 10;
    int chunk_size = file_size / NUM_THREADS;
    int total_sum = 0;

    pthread_t tids[NUM_THREADS];
    pthread_attr_t attr[NUM_THREADS];

    // instances of the VectorThreadData struct to be added to total_sum global var when threads complete
    VectorThreadData multi_thread_vector_data[NUM_THREADS];
    VectorThreadData single_thread_data = { nums, 0 };

    cout << "\n|\t\t*** Vector solution ***\t\t\t|\n" << endl;

    // Calculate sum with 1 thread using vectors
    auto start = high_resolution_clock::now();
        total_sum = 0;
        pthread_attr_init(&attr[0]);
        pthread_create(&tids[0], &attr[0], add_vector_to_local_sum, &single_thread_data);

        pthread_join(tids[0], NULL);
        total_sum = single_thread_data.local_sum;
        auto stop = high_resolution_clock::now();
        auto single_duration = duration_cast<microseconds>(stop - start);
        cout << "Calculated sum (1 thread): \t" << total_sum << endl;
        cout << "1 thread runtime: \t\t" << single_duration.count() << " microseconds" << endl;

    // Calculate sum with 10 threads using vectors
    start = high_resolution_clock::now();
    total_sum = 0;

        for (int j = 0; j < NUM_THREADS; j++) {
            multi_thread_vector_data[j].nums = slicing(nums, j * chunk_size, (j + 1) * chunk_size);
            pthread_attr_init(&attr[j]);
            pthread_create(&tids[j], &attr[j], add_vector_to_local_sum, &multi_thread_vector_data[j]);
        }

        for (int j = 0; j < NUM_THREADS; j++) {
            pthread_join(tids[j], NULL);
            total_sum += multi_thread_vector_data[j].local_sum;
        }
        stop = high_resolution_clock::now();
        auto threaded_duration = duration_cast<microseconds>(stop - start);
        cout << "Calculated sum (10 threaded): \t" << total_sum << endl;
        cout << "10 thread runtime: \t\t" << threaded_duration.count() << " microseconds" << endl;

   
    cout << "\n|\t\t*** Array solution ***\t\t\t|\n" << endl;
   
    int array_size = file_size;
    int single_thread_array[file_size];
    int multithread_matrix[NUM_THREADS][chunk_size];

    // Populate arrays
    for (int i = 0; i < file_size; i++) {
        single_thread_array[i] = nums[i];
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        for (int j = 0; j < chunk_size; j++) {
            multithread_matrix[i][j] = single_thread_array[i * chunk_size + j];
        }
    }

    ArrayThreadData single_thread_array_data = {array_size, &single_thread_array[0], 0};
    ArrayThreadData multi_thread_array_data[NUM_THREADS];

    // Calculate sum with 1 thread using arrays
    total_sum = 0;
    start = high_resolution_clock::now();
    pthread_attr_init(&attr[0]);
    pthread_create(&tids[0], &attr[0], add_array_to_sum, &single_thread_array_data);

    pthread_join(tids[0], NULL);
    total_sum = single_thread_array_data.local_sum;
    stop = high_resolution_clock::now();
    auto single_thread_array_duration = duration_cast<microseconds>(stop - start);

    cout << "Calculated sum (1 thread): \t" << total_sum << endl;
    cout << "1 thread runtime: \t\t" << single_thread_array_duration.count() << " microseconds" << endl;

    // Calculate sum with 10 threads using arrays
    total_sum = 0;
    array_size = chunk_size;

    start = high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; i++) {
        multi_thread_array_data[i] = { chunk_size, &multithread_matrix[i][0] , 0};
        pthread_attr_init(&attr[i]);
        pthread_create(&tids[i], &attr[i], add_array_to_sum, &multi_thread_array_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tids[i], NULL);
        total_sum += multi_thread_array_data[i].local_sum;
    }
    stop = high_resolution_clock::now();
    auto multi_matrix_duration = duration_cast<microseconds>(stop - start);

    cout << "Calculated sum (10 threads): \t" << total_sum << endl;
    cout << "10 thread runtime: \t\t" << multi_matrix_duration.count() << " microseconds\n" << endl;

    return 0;
}
