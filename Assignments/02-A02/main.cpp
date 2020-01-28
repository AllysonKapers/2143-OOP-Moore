///////////////////////////////////////////////////////////////////////////////
//                   
// Author:           Ally Moore
// Email:            ally_moore6@hotmail.com
// Title:            Commenting Homework
// Course:           CMPS 2143
// Semester:         Spring 2020
//
// Description:
//       This program implements a class that allows a linked list to be used normally and with
//       just like an array. It overloads the "[]" (square brackets) to simulate accessing 
//       seperate array elements, but really it traverses the list to find the specified node 
//       using an index value. This program also has a method to concatenate lists and print
//       a list to a file.
//       
// Files:            
//      main.cpp    : driver program 
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

using namespace std;

int A[100];

struct Node
{
    int x;          //data type held in node
    Node *next;     //pointer to keep track of next node
    //constructor
    Node()
    {
        x = -1;             //default data is -1
        next = NULL;        //next will alwasy be NULL unless manipulated otherwise
    }
    //overloaded constructor
    Node(int n)
    {
        x = n;              //node data is whatever int is passed into the overloaded constructor
        next = NULL;
    }
};

/**
 * List
 * 
 * Description:
 *      This is a basic List class with all the basic functions, plus functions to 
 *      concatenate lists, search for an index like an an array, and print to a file.
 * 
 * Public Methods:
 *      - List 
 *      - void Push(int val)
 *      - void Insert(int val)
 *      - void PrintTail()
 *      - string Print()
 *      - int Pop()
 *      - List operator+(const List &Rhs)
 *      - int operator[](int index)
 *      - friend ostream &operator<<(ostream &os, List L)
 * 
 * Private Methods:
 *      - Node *Head;
 *      - Node *Tail;
 *      - int Size;
 * 
 * Usage: 
 * 
 *      - examples of how
 *      - to use your class 
 *      
 */

class List
{
  private:
    Node *Head;     //pointer to keep track of Head
    Node *Tail;     //pointer to keep track of Tail
    int Size;       //int to keep track of the size of the list

  public:
    //class constructor
    List()
    {
        Head = Tail = NULL;
        Size = 0;
    }

    /**
     * Public/Private/Protected : push
     * 
     * Description:
     *      Adds new node to the end of the list, handles case of first node
     * 
     * Params:
     *      - int val : integer to be loaded into a new node
     * 
     * Returns:
     *      - nothing
     */
    void Push(int val)
    {
        // allocate new memory and init node
        Node *Temp = new Node(val);

        if (!Head && !Tail)
        {
            Head = Tail = Temp;
        }
        else
        {
            Tail->next = Temp;
            Tail = Temp;
        }
        Size++;
    }

    /**
     * Public/Private/Protected : Insert
     * 
     * Description:
     *      Adds a new node to the front of the list
     * 
     * Params:
     *      - int val : loads val into a new node
     * 
     * Returns:
     *      - nothing 
     */
    void Insert(int val)
    {
        // allocate new memory and init node
        Node *Temp = new Node(val);

        // figure out where it goes in the list

        Temp->next = Head;
        Head = Temp;
        if (!Tail)
        {
            Tail = Head;
        }
        Size++;
    }

    /**
     * Public/Private/Protected : PrintTail
     * 
     * Description:
     *     Prints the data in the Tail node
     * 
     * Params:
     *      - none
     * 
     * Returns:
     *      - nothing
     */
    void PrintTail()
    {
        cout << Tail->x << endl;
    }

    /**
     * Public/Private/Protected : Print
     * 
     * Description:
     *      Traverses the list adding each value to a string
     * 
     * Params:
     *      - none
     * 
     * Returns:
     *      - string list
     */
    string Print()
    {
        Node *Temp = Head;
        string list;

        while (Temp != NULL)
        {
            list += to_string(Temp->x) + "->";
            Temp = Temp->next;
        }

        return list;
    }

    /**
     * Public/Private/Protected : Pop
     * 
     * Description:
     *      Would remove a node from the list
     * 
     * Params:
     *      - none
     * 
     * Returns:
     *      - 0 : (would return popped nodes data if implemented)
     */
    // not implemented 
    int Pop()
    {
        Size--;
        return 0; //
    }

    /**
     * Public/Private/Protected : operator+
     * 
     * Description:
     *      adds a passed in list to the end of the local list
     * 
     * Params:
     *      - const List &Rhs : list to be concatenated to local list, holds ints
     * 
     * Returns:
     *      - list NewList
     */
    List operator+(const List &Rhs)
    {
        // Create a new list that will contain both when done
        List NewList;

        // Get a reference to beginning of local list
        Node *Temp = Head;

        // Loop through local list and Push values onto new list
        while (Temp != NULL)
        {
            NewList.Push(Temp->x);
            Temp = Temp->next;
        }

        // Get a reference to head of Rhs
        Temp = Rhs.Head;

        // Same as above, loop and push
        while (Temp != NULL)
        {
            NewList.Push(Temp->x);
            Temp = Temp->next;
        }

        // Return new concatenated version of lists
        return NewList;
    }

    /**
     * Public/Private/Protected : operator[]
     * 
     * Description:
     *      Treats list like an array and returns the int value at a specified index
     * 
     * Params:
     *      - int index : the locations we're looking for in the list
     * 
     * Returns:
     *      - int : returns the integer value at 
     */
    // Implementation of [] operator.  This function returns an
    // int value as if the list were an array.
    int operator[](int index)
    {
        Node *Temp = Head;

        if (index >= Size)
        {
            cout << "Index out of bounds, exiting";
            exit(0);
        }
        else
        {

            for (int i = 0; i < index; i++)
            {
                Temp = Temp->next;
            }
            return Temp->x;
        }
    }

    /**
     * Public : &operator<<
     * 
     * Description:
     *      prints a passed list to a file
     * 
     * Params:
     *      ostream &os    :  the file the user wants the list printed to
     *      List L     :  the list to be printed to the file
     * 
     * Returns:
     *      friend   : the modified file that was passed in
     */
    friend ostream &operator<<(ostream &os, List L)
    {
        os << L.Print();
        return os;
    }
};

int main(int argc, char **argv)
{
    List L1;
    List L2;

    for (int i = 0; i < 25; i++)
    {
        L1.Push(i);
    }

    for (int i = 50; i < 100; i++)
    {
        L2.Push(i);
    }

    //cout << L1 << endl;
    L1.PrintTail();
    L2.PrintTail();

    List L3 = L1 + L2;
    cout << L3 << endl;

    cout << L3[5] << endl;
    return 0;
}