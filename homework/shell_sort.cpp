/*
7.1 Sort the sequence 3, 1, 4, 1, 5, 9, 2, 6, 5 using insertion sort. Show each iteration.
7.2 What is the running time of insertion sort if all elements are equal?
7.15 Sort 3, 1, 4, 1, 5, 9, 2, 6 using mergesort. Show each iteration.
7.19 Sort 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 using quicksort with median-of-three partitioning and a cutoff of 3. Show each iteration.
*/


#include <iostream>
#include <vector>

using namespace std;

void printMyList( vector<int> & a)
{
	int length = a.size();
	if (length == 0)
	{
		cout<<"[empty]"<<endl;
		return;
	}
	cout << '[';
	for (int i = 0; i<length; i++)
	{
		cout << a[i];
		if (i <length-1)
			cout << ',';
	}
	cout << ']' << endl;
}

void insertionSort(vector<int> & a)
{
	int length = a.size();
	int j;
	for( int p = 1; p<a.size() ; p++)
	{
		int temp = a[p];
		for( j = p; j > 0 && temp < a[j-1]; j--)
		{
			a[j] = a[j-1];
			printMyList(a);
		}
		a[j] = temp;
		// printMyList(a);
	}
}

//merge routine
void merge(vector<int> & a, vector<int> & temp, int left_p, int right_p, int right_end)
{
	int left_end = right_p - 1;
	int temp_p = left_p;
	int num_elems = right_end - left_p + 1;

	while (left_p <= left_end && right_p <= right_end)
	{
		if (a[left_p] <= a[right_p])
			temp[temp_p++] = a[left_p++];
		else
			temp[temp_p++] = a[right_p++];
	}
	while (left_p <= left_end)
		temp[temp_p++] = a[left_p++];
	while (right_p <= right_end)
		temp[temp_p++] = a[right_p++];
	// printMyList(a);
	// printMyList(temp);
	for (int i = 0; i<num_elems; i++, right_end--)
		a[right_end] = temp[right_end];
}
// merge sort logic
void merge_sort(vector<int> & a, vector<int> & temp, int left, int right)
{
	printMyList(a);
	if (left < right)
	{
		int center = (left+right)/2;
		merge_sort(a, temp, left, center);
		merge_sort(a, temp, center+1, right);
		merge(a, temp, left, center+1, right);
	} 
	// printMyList(a);
}
// driver function
void merge_sort ( vector<int> & a )
{
	vector<int> temp(a.size());
	merge_sort(a, temp, 0, a.size()-1);
	// printMyList(a);
}

int median3(vector<int> &a, int left, int right)
{
	int center = (left + right) / 2;
	if(a[center] < a[left])
		swap(a[left], a[center]);
	if(a[right] < a[right])
		swap(a[left], a[right]);
	if(a[right] < a[center])
		swap(a[center], a[right]);
	swap(a[center], a[right-1]);
	return a[right-1];
}

// quick sort routine
void quick_sort(vector<int> & a, int left, int right)
{
	printMyList(a);
	if (left+3 == right)
	{
		int pivot = median3(a, left, right);
		int i, j;
		i = left;
		j = right-1;
		for( ; ; )
		{
			while(a[++i]<pivot){}
			while(a[--j]>pivot){}
			if (i < j)
				swap(a[i], a[j]);
			else
				break;
		}
		swap(a[i], a[right-1]);
		quick_sort(a, left, i-1);
		quick_sort(a, i+1, right);
	}
	else
		insertionSort(a);
}

void quick_sort(vector<int> & a)
{
	quick_sort(a, 0, a.size()-1);
}
// the sequence 3, 1, 4, 1, 5, 9, 2, 6, 5
void initV1(vector<int> & v1)
{
	v1[0] = 3;
	v1[1] = 1;
	v1[2] = 4;
	v1[3] = 1;
	v1[4] = 5;
	v1[5] = 9;
	v1[6] = 2;
	v1[7] = 6;
	v1[8] = 5;
}
// what if all of the elements were the same!?
void initVSame(vector<int> & v)
{
	for(int i=0; i<v.size(); i++)
	{
		v[i] = 1;
	}
}

//3, 1, 4, 1, 5, 9, 2, 6 using merge sort
void initV2(vector<int> & v2)
{
	v2[0] = 3;
	v2[1] = 1;
	v2[2] = 4;
	v2[3] = 1;
	v2[4] = 5;
	v2[5] = 9;
	v2[6] = 2;
	v2[7] = 6;
}
// Sort 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 using quicksort
void initV3(vector<int> & v2)
{
	v2[0] = 3;
	v2[1] = 1;
	v2[2] = 4;
	v2[3] = 1;
	v2[4] = 5;
	v2[5] = 9;
	v2[6] = 2;
	v2[7] = 6;
	v2[8] = 5;
	v2[9] = 3;
	v2[10] = 5;
}
int main()
{
	vector<int> v1(9);
	vector<int> v2(8);
	vector<int> v3(11);
	cout << "can declare vectors..." << endl;
	initV1(v1);
	initV2(v2);
	initV3(v3);
	cout << "can init vectors" << endl;

	cout << "before insertion sort on v1:" << endl;
	printMyList(v1);
	cout << "iterating thru insertions & printing each iteration..." << endl;
	insertionSort(v1);
	initVSame(v1);
	cout << "okay.... we've now made all of the elements the same.\nHow will insertion sort behave now?" << endl;
	cout << "our list of same elements: ";
	printMyList(v1);
	insertionSort(v1);

	cout << "before insertion sort on v2:" << endl;
	printMyList(v2);
	cout << "iterating through merge sort & printing each iteration" << endl;
	merge_sort(v2);
	cout << "wow! okay.... how about all same elements in merge sort?" << endl;
	initVSame(v2);
	cout << "Vector: "; printMyList(v2);
	merge_sort(v2);
	cout << "Fascinating!";

	cout << "okay now we are going to run quick sort!" << endl;
	cout << "our unsorted list: " ; printMyList(v3);
	cout << "sorting and printing each iteration..."<<endl;
	quick_sort(v3);

	return 0;
} 
