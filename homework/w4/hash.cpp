#include <iostream>
#include <vector>
#include <utility> 

using namespace std;

const int INITIAL_TABLE_SIZE = 7;

// class to store key value pairs. missing in starter code
class HashEntry {
public:
    int key;
    int value;
    HashEntry(int key, int value) {
        this->key = key;
        this->value = value;
    }
};

class HashMap {
private:
    // a vector of key value stores
    vector<HashEntry*> table;

    int capacity; // capacity of the hash map
    int size; // actual size ofthe hash map

    // very basic hash function using modulo. 
    int hashFunction(int key) {
        return key % capacity;
    }


    void rehash() {
        cout << "OH NO! we have to make more room!" << endl;
        // assign old capacity to variable so capacity can be doubled
        int oldCapacity = capacity;
        capacity *= 2;
        cout << "capacity raised from: " << oldCapacity << " to: " << capacity << endl;
        // assign old table to a variable so we can copy it into the new table
        vector<HashEntry*> oldTable = table;

        // intitialize pointer to new table first address and enough memory for resized table
        table = vector<HashEntry*>(capacity, nullptr);
        size = 0;
        
        // copy contents of old table into resized table
        for (int i = 0; i < oldCapacity; ++i) {
            if (oldTable[i] != nullptr) {
                insert(oldTable[i]->value);
                delete oldTable[i];
            }
        }
    }

public:
    // basic constructor
    HashMap(int initialCapacity) {
        capacity = initialCapacity;
        table.resize(capacity, nullptr);
        size = 0;
    }

    //accessor funciton fixed. previously did not handle unfound keys.
    int get(int key) {
        int hash = hashFunction(key);
        while (table[hash] != nullptr && table[hash]->key != key) {
            hash = (hash + 1) % capacity;
        }
        if (table[hash] == nullptr)
            return -1;
        else
            return table[hash]->value;
    }


    void insert(int value) {
        // check size, rehash if insertion results in too big a table
        if (size >= capacity / 2) {
            rehash();
        }

        // hash the value to be inserted
        int hash = hashFunction(value);
        // iterate through table values. first check if key is null pointer. if not null, check value match. if not, increment hash and mod against cap. recheck until all checks pass. 
        while (table[hash] != nullptr && table[hash]->key != value) {
            hash = (hash + 1) % capacity;
        }


        if (table[hash] == nullptr) {
            table[hash] = new HashEntry(value, value); // both key and value are integers. this is a pretty silly hash table.
            size++;
        } else {
            table[hash]->value = value;
        }
    }
    // basic destructor
    ~HashMap() {
        for (int i = 0; i < capacity; ++i) {
            if (table[i] != nullptr) {
                delete table[i];
            }
        }
    }

    void displayTable() {
        for (int i = 0; i < capacity; ++i) {
            if (i==0)
                cout << "idx: (key, val)"<< endl;
            if (table[i] != nullptr) {
                cout << i << ": (" << table[i]->key << ", " << table[i]->value << ")" << endl;
            } else {
                cout << i << ": (empty)" << endl;
            }
        }
    }
};

int main() {
    

    int values[] = {138, 99, 16, 134, 42, 0, 6, 9, 4, 53, 47, 66};
    HashMap hashMap1(17);

    // Insert the sequence into a table of size 17
    for (int value : values) {
        hashMap1.insert(value);
    }

    cout << "Hash Table with initial size 17:" << endl;
    hashMap1.displayTable();
    cout << endl;

    // Task 1.2: Rehashing with an initial table size of 7
    HashMap hashMap2(INITIAL_TABLE_SIZE);


    for (int value : values) {
        hashMap2.insert(value);
    }

    cout << "Hash Table after rehashing with initial size 7:" << endl;
    hashMap2.displayTable();

    return 0;
}
