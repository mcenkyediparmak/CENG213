#include <iostream>

#include "LinkedList.h"

int main()
{
    LinkedList<int> llist;

    llist.print();

    llist.insertAtTheEnd(5);
    llist.insertAtTheFront(2);
    llist.insertAtTheEnd(3);

    llist.insertAfterNode(1, llist.getNode(5));

    llist.insertAsEveryKthNode(9, 3);

    std::cout << std::endl;
    llist.shuffle(2);
    LinkedList<int> turko;
    turko.insertAtTheEnd(3);
    turko = llist;
    return 0;
}
