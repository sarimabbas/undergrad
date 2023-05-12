// ------------------------------------------------------------------------
// Demonstration of derived classes with virtual functions.
// A. Fischer  June 15, 1998                                 file: main.cpp
// ------------------------------------------------------------------------
#include "exam.hpp"
#include "stack.hpp"
#include "queue.hpp"
#include "item.hpp"
// ------------------------------------------------------------------------
int main( void ) {
    Stack s;
    Queue q;

    cout << "\nPutting 3 items on the Stack S: 99, 88, 77.\n" ;
    s.put( new Item("Ned", 99) );       //cout << S << endl;
    s.put( new Item("Max", 88) );       //cout << S << endl;
    cout << "  Peeking after second insertion: " <<*s.peek() <<"\n";
    s.put( new Item("Leo",77) );          cout << s << endl;

    cout << "Putting 3 items on the Queue Q: 11, 22, 33.\n";
    q.put( new Item("Ali",11) );        //cout << Q << endl;
    q.put( new Item("Bea",22) );        //cout << Q << endl;
    cout << "  Peeking after second insertion: " <<*q.peek() <<"\n";
    q.put( new Item("Cil",33) );          cout << q << endl;

    cout << "Pop two Exams from Q, put on S. \n";
    s.put(q.pop()); s.put(q.pop());       cout <<"\n" <<s << endl;

    cout << "Put another Item onto Q: 44.\n";
    q.put( new Item("Dan",44) );          cout << q << endl;

    cout << "Pop two Exams from S and discard.\n";
    delete s.pop();
    delete s.pop();                       cout <<"\n" << s << endl;
}
