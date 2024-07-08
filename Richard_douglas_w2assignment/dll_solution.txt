/**
 * A sample code of Creating C++ linked lists,
 * Including definitions the list node class and linked list class,
 * and how to create a blank linked list and a one-node linked list.
 *
 * Outline: understand the definition and structure of the linked
 * list and build a linked list based on it.
 */

#include <iostream>
#include <cstddef>

using std::cout;
using std::endl;




/* definition of the list node class */
class Node
{
    friend class LinkedList;
private:
    int value; 
    Node *pNext; 
    Node *pPrev;
public:
    /* Constructors with No Arguments */
    Node(void)
    : pNext(NULL)
    { }
    
    /* Constructors with a given value */
    Node(int val)
    : value(val), pNext(NULL)
    { }
    
    /* Constructors with a given value and a link to the next node */
    Node(int val, Node* next)
    : value(val), pNext(next)
    {}

    /* Constructors with a given value and a link to the previous (parent) and next node */
    Node(int val, Node* prev, Node* next)
    : value(val), pPrev(prev), pNext(next)
    {}
    
    /* Getters */
    int getValue(void)
    { return value; }
    
    Node* getNext(void)
    { return pNext; }
};

/* definition of the linked list class */
class LinkedList
{
private:
    /* pointer of head node */
    Node *pHead;
    /* pointer of tail node */
    Node *pTail;
    
public:
    /* Constructors with No Arguments */
    LinkedList(void);
    /* Constructors with a given value of a list node */
    LinkedList(int val);
    /* Destructor */
    ~LinkedList(void);
    
    /* Traversing the list and printing the value of each node */
    void traverse_and_print();
	void traverse_and_print_nodes();
	void push_back(int val);
};

LinkedList::LinkedList()
{
    /* Initialize the head and tail node */
    pHead = pTail = NULL;
}

LinkedList::LinkedList(int val)
{
    /* Instead (like in the lectures and example code) initialize head and tail as empty nodes */
    /* then create a new node with the first value of list. Set pointers to the empty, head and tail*/
    /* finally, reset the head pNext pointer to the first real Node and the tail's pPrev pointer to the same*/
    pHead = new Node();
    pTail = new Node();
    Node *firstReal = new Node(val, pHead, pTail);
    pHead->pNext = firstReal;
    pTail->pPrev = firstReal;
}

LinkedList::~LinkedList()
{
    
}

void LinkedList::traverse_and_print()
{
    Node *p = pHead;
    
    /* The list is empty? */
    if (pHead == NULL) {
        cout << "The list is empty" << endl;
        return;
    }
    
    cout << "LinkedList: ";
    /* A basic way of traversing a linked list */
    while (p != NULL) { /* while there are some more nodes left */
        if (p == pHead)
        {
            cout<< "[ ";
            p = p->pNext;
            continue;
        }
        if (p == pTail)
        {
            cout << " ]";
            p = p->pNext;
            continue;
        }
        
        /* output the value */
        cout << p->value;
        
        if (p->pNext != pTail)
        {
            cout << ", ";
        }
        /* The pointer moves along to the next one */
        p = p->pNext;
    }
    cout << endl;
}

void LinkedList::traverse_and_print_nodes()
{
    Node *p = pHead;
    
    /* The list is empty? */
    if (pHead == NULL) {
        cout << "The list is empty" << endl;
        return;
    }
    
    cout << "LinkedList: ";
    /* A basic way of traversing a linked list */
    while (p != NULL) { /* while there are some more nodes left */
        if (p == pHead)
        {
            cout<< "[\n";
        }
        
        /* output the whole node */
        cout << "\tvalue: " << p->value << endl;
        cout << "\tpPrev: " << p->pPrev << endl;
        cout << "\tpNext: " << p->pNext << endl;
        cout << "\tmemLocation: " << p << endl;
        
        cout << "\n";

        if (p == pTail)
        {
            cout << " ]";
        }
        /* The pointer moves along to the next one */
        p = p->pNext;
    }
    cout << endl;
}

void LinkedList::push_back(int val){
	/*Your code here*/
	Node *newNode = new Node(val);
    newNode->pPrev = pTail->pPrev;
    newNode->pNext = pTail;
    pTail->pPrev->pNext = newNode;
    pTail->pPrev = newNode;
}

int main(int argc, const char * argv[])
{
    /* Create an empty list */
    LinkedList list1;
    cout << "Created an empty list named list1." << endl;
    /* output the result */
    cout << "list1:" << endl;
    list1.traverse_and_print();
    // traverse and print the whole node to display the pointers and memory location
    list1.traverse_and_print_nodes();
    
    /* Create a list with only one node */
    LinkedList list2(10);
    cout << "Created a list named list2 with only one node." << endl;
    /* output the result */
    cout << "list2:" << endl;
    list2.traverse_and_print();
    // traverse and print the whole node to display the pointers and memory location
    list2.traverse_and_print_nodes();
	
	/*your testing code here*/
	for (int i = 0 ; i < 10; i++){
		list2.push_back(i);
	}
	list2.traverse_and_print();
    // traverse and print the whole node to display the pointers and memory location
    list2.traverse_and_print_nodes();
    return 0;
}


