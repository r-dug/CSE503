/*
Author: 
    Richard Douglas

Description:
    Binary search tree implementation using integers as the value data type. 
    I'm sorry I didn'tstay closer to your implementation, using functions independent of a BST class.
    However, I wanted to write out the code shown in the textbook so I could understand it better...
    And OHHHHH boy do I now uynderstand it better. I thoroughly enjoyed the exercise.
    I have to say that this really clarified my understanding of C++, and the ADT binary tree.

Date created: 
    Jun 2, 2024
Last modified:
    Jun 2, 2024
*/

#include <iostream>
#include <cstddef>
#include <chrono>
#include <thread>

using std::cout;
using std::endl;
using std::string;

class Node 
{
    private:
        int value;
    public:
        Node* left;       // left child
        Node* right;      // right child
        Node* p;          // parent
        Node(int data) {
            value = data;
            left = NULL;
            right = NULL;
            p  = NULL; // in this homework I never actually USE the parent node. In this way, my BST implementation is "singly-linked", but that seems to be fine for the purposes of the assignment.
        }
        ~Node() {
        }
        int d() {
            return value;
        }
        void assign_val(int d)
        {
            value = d;
        }
        void print() {
            std::cout << value << std::endl;
        }
};

class BST 
{
    public:
        //constructors
        BST(): root(nullptr) {};
        BST(Node* rootNode): root(rootNode) {};
        //destructor
        ~BST(){ makeEmpty();};

        //constant public member functions reference private member functions. Root is pvt.
        const int & findMin() const;
        const int & findMax() const;
        
        bool search(const int & x) const
        {
            return search( x, root );
        }
        void print_tree() const
        {
            print_tree( root, 0 );
        }
        void print_details() const
        {
            print_details(root, 0);
        }
        void insert( const int & x )
        {
            insert( x, root, nullptr );
        }
        void delete_node( const int & x )
        {
            delete_node( x, root );
        }
        void makeEmpty()
        {
            makeEmpty( root );
        }


        // assignment operator overloading for copying
        const BST & operator=( const BST & rhs );

    private:
    /*
        Obfuscating the private functions here really didn't click with me at first... 
        I had to code this out to understand that you want to pass in the root 
            and only have the option to pass in that node when modifyingthe BST.
    */
        Node *root;

        Node * findMin( Node *t ) const;
        Node * findMax( Node *t ) const;

        bool search( const int & x, Node *t ) const;

        void print_tree( Node *t, int indent ) const;
        void print_details( Node *t, int indent ) const;
        void insert( const int & x, Node * & t , Node * p ) const; 
        void delete_node( const int & x, Node * & t ) const;
        void makeEmpty(Node * & t); // necessary for destruction

        Node * clone( Node *t ) const;

};
Node * BST::findMin(Node *t ) const
{
    //empty tree check
    if ( t == NULL )
        return NULL;
    // if pointer to left is null, there is no lesser value node
    if ( t->left == NULL)
        return t;
    //recursive function call passing the left (lesser) adjacent node
    return findMin( t->left );
}
Node * BST::findMax(Node *t ) const
{
    //check that the tree has values / is not empty
    if ( t != NULL )
    {   
        //loop continues to rightmost node, which necessarily contains the max value.
        while ( t->right == NULL )
            t = t->right;
    }
    // return the righmost node (max)
    return t;
}

bool BST::search( const int & x, Node *t) const 
{
    if ( t==NULL )
        return false; // if an empty tree is found, false
    else if ( x < t->d() )
        return search( x, t->left ); // search val less than current node. look left.
    else if ( t->d() < x )
        return search( x, t->right ); // search val greater than current node. look right.
    else
        return true; // match found! total evaluation will always be true.
} 

void BST::print_tree( Node *t, int indent) const
{
    /*
    Overall logic:
        print every node in the tree recursively. 
            For clarity, I want the printed output of every node to include the memory addresses of the node, its parent, and its children....
        When a node has NULL pointers for left and right, you'll have reached a leaf, and recursion can stop.
    */
    string indentation = string(indent, ' ');
    
    if (t != NULL) {
        if (t->p == NULL) // check for no parents. printed at begining of recursive calls.
            cout << "\x1B[31m" << "'*' marks the ROOT node & tree reads left to right\n\n" << "\x1B[0m"; //marks the root
        if (t->right != NULL) // recursive calls at this check end when the rightmost (greatest val) node is found
        {
            print_tree(t->right, indent + 4); //incremented indentation recursive call
        }
        if (indent) {
            cout << indentation;
        }
        if (t->right != NULL) {
            cout << "\x1B[36m" << "  /\n" << "\x1B[0m" << indentation; // if not the rightmost node, print a '/' representing link. newline and indent
        }
        if (t->p == NULL)
            cout << "\x1B[31m" << "*" << "\x1B[0m"; //marks the root
        cout << t->d() << "\n";
        if (t->left != NULL) {
            cout << indentation << "\x1B[36m" << "  \\\n" << "\x1B[0m";
            print_tree(t->left, indent + 4);
        }
        if (t->left != NULL && t->right != NULL)
            cout << endl;
    }
}
void BST::print_details(Node *t, int indent) const
{
    /*
    Overall logic:
        print every node in the tree recursively. 
            For clarity, I included the memory addresses of the node, its parent, and its children (if it has parent or child)....
        When a node has NULL pointers for left and right, you'll have reached a leaf, and recursion can return.
        I also may have spent too much time on this function.... but it looks kinda nice :)
    */
    string indentation = string(indent, '\t');
    string border = string(20, '*');
    if (indent == 0)
        cout << "\x1B[31m" << "ROOT" << "\x1B[0m" << endl;
    if (t->d())
    {
        cout << indentation << "\x1B[36m" << border << "\x1B[0m" << endl;
        cout << indentation << "Node ptr: " << t << endl;
        cout << indentation << "\tNode val: " << t->d() << endl;
        if (t->p)
        {
            cout << indentation << "Parent node ptr: " << t->p << endl;
            cout << indentation << "\tParent node val: " << t->p->d() << endl;
        } else 
        {
            cout << indentation<< "\x1B[31m"  << "This is the  root! No parent!"<< "\x1B[0m" << endl;
        }
        if (t->right)
        {
            cout << indentation << "Right child ptr: " << t->right << endl;
            cout << indentation << "\tRight child val: " << t->right->d() << endl;
        } else 
        {
            cout << indentation << "No right child!"<< endl;
        }
        if (t->left)
        {
            cout << indentation << "Left child ptr: " << t->left << endl;
            cout << indentation << "\tLeft child val: " << t->left->d() << endl;
        }else 
        {
            cout << indentation << "No left child!" << endl;
        }
        cout << indentation << "\x1B[36m" << border << "\x1B[0m" << endl << endl;
    }
    if (t->right != NULL)
    {   
        indent++;
        print_details(t->right, indent);
    }
    if (t->left != NULL)
    {   
        indent++;
        print_details(t->left, indent);
    }
}
void BST::insert(const int & x, Node * & t,  Node * p) const 
{
    /*
    OVERALL LOGIC:
        Recursively call insert until the place where 'x' fits into the tree is found
        then, in the case where pointer passed into the function call points to NULL,
        the new node is created and inserted as the left or right node pointed to.
        
        The last case is the result of the dereferenced pointer being neither NULL value, greater than 'x', or less than 'x'.
        This implies we have found a node with a value equal to x, so we do NOTHING. Recursion stops, and no node is created to avoid duplication in the tree.
    */

    // empty tree check... For me it's helpful to keep in mind that the whole tree is a bunch of subtrees...
    if ( t == NULL)
    {
        t = new Node( x );
        t->p = p;
    }
    // less than check
    else if ( x < t->d() )
        insert( x, t->left, t );
    // greater than check
    else if ( t->d() < x )
        insert( x, t->right, t );
    // implicit equality check... could be done explicitly, but it's unnecessary.
    else
        cout << "tried to insert a duplicate" << x << ". no duplicates!" << endl; //do nothing
}
void BST::delete_node(const int & x, Node * & t) const
{
    /*
    This one is actually a little funky.
    */
    if ( t == NULL ) // obvi
    {
        cout << x << " not found, so could not be deleted." << endl;
        return;
    }
    if ( x < t->d() ) // delete val lt node val. look left.
        delete_node( x, t->left);
    else if ( t->d() < x ) // delete val gt node val. look right.
        delete_node( x, t->right);
    else if ( t->left != NULL && t->right != NULL )
    {
        t->assign_val(findMin(t->right)->d()); // reassign current node val to the min of the right child tree (necessarily still gt current node  val)
        delete_node(t->d(), t->right); // delete min node in right subtree. essentially, swapping the least val gt current node with current node val. slick.
    }
    else // only remaining case - delete value found and only one subtree exists from that node.
    {
        Node *old = t; // copy pointer to current node for later space freeing.
        t = (t->left != NULL) ? t->left : t->right; // if left pointer is not NULL, assign that object to current node. else, assign the right. i.i assign the only subtree to the current node.
        delete old; // free memory from old node (deleting it)
    }
}
void BST::makeEmpty( Node * & t )
{
    if ( t != NULL )
    {
        makeEmpty(t->right);
        makeEmpty(t->left);
        delete t; // the order is important here. recursively call all of the nodes and delete them.
    }
    t = NULL;
}
void console_clear_screen() {
  #ifdef _WIN364
  system("cls");
  #endif
  #ifdef __linux__
  system("clear");
  #endif
  #ifdef __APPLE__
  system("clear");
  #endif
  #ifdef __unix__ 
  system("clear");
  #endif
}
int main(int argc, const char * argv[])
{
    console_clear_screen();
    BST new_tree;
    int nums[] = {3, 2, 5, 6, 10, 4, 7, 8, 11, 15, 13};
    int i = 0;
    using std::operator""s;
    while(i<sizeof(nums) / sizeof(nums[0]))
    {
        new_tree.insert(nums[i]);
        cout << "Our Tree after inserting " << nums[i] << ':' << endl;
        new_tree.print_tree();
        i++;
        std::this_thread::sleep_for(1s);
        console_clear_screen();
    }
    i = 0;
    while(i<sizeof(nums) / sizeof(nums[0]))
    {
        new_tree.insert(nums[i]);
        cout << "Our Tree after inserting " << nums[i] << ':' << endl;
        new_tree.print_tree();
        i++;
        std::this_thread::sleep_for(1s);
        console_clear_screen();
    }
    std::this_thread::sleep_for(1s);
    if (new_tree.search(7))
    {
        cout << "found 7" << endl;
    }
    std::this_thread::sleep_for(1s);
    if (new_tree.search(5))
    {
        cout << "found 5" << endl;
    }
    std::this_thread::sleep_for(1s);
    new_tree.delete_node(5);
    cout << "Our Tree after deleting 5:" << endl;
    new_tree.print_tree();

    std::this_thread::sleep_for(1s);
    new_tree.delete_node(8);
    cout << "Our Tree after deleting 8:" << endl;
    new_tree.print_tree();

    std::this_thread::sleep_for(1s);
    new_tree.delete_node(500);
    cout << "Final details of our BST:" << endl;

    std::this_thread::sleep_for(1s);
    new_tree.print_details();
    return 0;
}
