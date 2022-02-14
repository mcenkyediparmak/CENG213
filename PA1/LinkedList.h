#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

template <class T>
class LinkedList
{
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &obj);

    ~LinkedList();

    int getSize() const;
    bool isEmpty() const;
    bool contains(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void insertAtTheFront(const T &data);
    void insertAtTheEnd(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAsEveryKthNode(const T &data, int k);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeAllNodes();
    void removeEveryKthNode(int k);

    void swap(Node<T> *node1, Node<T> *node2);
    void shuffle(int seed);

    void print(bool reverse = false) const;

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
private: // DO NOT CHANGE THIS PART.
    Node<T> *head;

    int size;
};

template <class T>
LinkedList<T>::LinkedList()
{
    head = NULL;

    size = 0;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj)
{
    size = 0;
    if (!obj.head)
    {
        head = NULL;
    }
    else
    {
        head = new Node<T>(obj.head->data);
        Node<T> *curr = head;
        Node<T> *copy = obj.head;
        while (copy->next != obj.head)
        {
            copy = copy->next;
            curr->next = new Node<T>(copy->data, curr, NULL);
            curr = curr->next;
            size++;
        }
        curr->next = head;
        head->prev = curr;
    }
}

template <class T>
LinkedList<T>::~LinkedList()
{
    Node<T> *curr = head;
    if (curr)
    {
        do
        {
            Node<T> *temp = curr;
            curr = curr->next;
            delete temp;
        } while (curr != head && curr);
        size = 0;
    }
}

template <class T>
int LinkedList<T>::getSize() const
{
    return size;
}

template <class T>
bool LinkedList<T>::isEmpty() const
{
    if (size)
    {
        return false;
    }
    return true;
}

template <class T>
bool LinkedList<T>::contains(Node<T> *node) const
{
    Node<T> *curr = head;
    if (!curr)
    {
        return false;
    }
    do
    {
        if (curr == node)
        {
            return true;
        }
        curr = curr->next;
    } while (curr != head);
    return false;
}

template <class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    if (head)
    {
        return head;
    }
    return NULL;
}

template <class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    if (head)
    {
        if (head->prev)
        {
            return head->prev;
        }
        return head;
    }
    return NULL;
}

template <class T>
Node<T> *LinkedList<T>::getNode(const T &data) const
{
    Node<T> *curr = head;
    do
    {
        if (data == curr->data)
        {
            return curr;
        }
        curr = curr->next;
    } while (curr != head);
    return NULL;
}

template <class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    Node<T> *curr = head;
    if (index > size - 1 || index < 0)
    {
        return NULL;
    }
    for (int i = 0; i < index; i++)
    {
        curr = curr->next;
    }
    return curr;
}

template <class T>
void LinkedList<T>::insertAtTheFront(const T &data)
{

    if (head)
    {
        Node<T> *inserted = new Node<T>(data);
        inserted->next = head;
        inserted->prev = head->prev;
        head->prev = inserted;
        head = inserted;
    }
    else
    {
        head = new Node<T>(data);
        head->next = head;
        head->prev = head;
    }

    size++;
}

template <class T>
void LinkedList<T>::insertAtTheEnd(const T &data)
{

    if (head)
    {
        Node<T> *inserted = new Node<T>(data);

        inserted->prev = head->prev;
        inserted->next = head;
        head->prev->next = inserted;
        head->prev = inserted;
    }
    else
    {
        head = new Node<T>(data);
        head->next = head;
        head->prev = head;
    }
    size++;
}

template <class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    Node<T> *inserted = new Node<T>(data);
    inserted->next = node->next;
    inserted->prev = node;
    node->next = inserted;
    inserted->next->prev = inserted;
    size++;
}

template <class T>
void LinkedList<T>::insertAsEveryKthNode(const T &data, int k)
{
    Node<T> *curr = head;
    int i = 0;
    if (k > 1 && k < (size + 1))
    {
        do
        {
            if ((i + 2) % k == 0)
            {
                insertAfterNode(data, curr);
                curr = curr->next;
                i++;
            }
            curr = curr->next;
            i++;

        } while (curr != head);
    }
}

template <class T>
void LinkedList<T>::removeNode(Node<T> *node)
{

    if (contains(node))
    {
        if (size == 1)
        {
            delete node;
            size--;
            head = NULL;
        }
        else
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            if (node == head)
            {
                head = node->next;
            }
        }
    }
}

template <class T>
void LinkedList<T>::removeNode(const T &data)
{
    Node<T> *curr = head;

    do
    {
        if (curr->data == data)
        {
            Node<T> *trash = curr;
            curr = curr->next;
            removeNode(trash);
        }
        else
        {
            curr = curr->next;
        }

    } while (curr != head);
}

template <class T>
void LinkedList<T>::removeAllNodes()
{
    Node<T> *curr = head;
    if (curr)
    {
        do
        {
            Node<T> *temp = curr;
            curr = curr->next;
            delete temp;
        } while (curr != head);
        size = 0;
    }
    head = NULL;
}

template <class T>
void LinkedList<T>::removeEveryKthNode(int k)
{
    Node<T> *curr = head;
    int i = 0;
    if (k > 1 && k < (size + 1))
    {
        do
        {
            if ((i + 1) % k == 0)
            {
                removeNode(curr);

                size--;
            }
            curr = curr->next;
            i++;

        } while (curr != head);
    }
}

template <class T>
void LinkedList<T>::swap(Node<T> *node1, Node<T> *node2)
{
    if (contains(node1) && contains(node2))
    {
        Node<T> *curr = node2;
        do
        {
            if (curr == node1)
            {
                Node<T> *tmp = node1;
                node1 = node2;
                node2 = tmp;
            }
            curr = curr->next;
        } while (curr != head);
        if (node1 == head)
        {

            head = node2;
        }

        Node<T> *temp = node2->next;
        Node<T> *temp2 = node1->prev;
        node2->next->prev = node1;
        node2->prev->next = node1;
        node1->next->prev = node2;
        node1->prev->next = node2;
        node2->next = node1->next;
        node1->next = temp;
        node2->prev = temp2;
        node1->prev = node2;
        temp2->next = node2;
    }
}

template <class T>
void LinkedList<T>::shuffle(int seed)
{
    for (int i = 0; i < size; i++)
    {
        swap(getNodeAtIndex(i), getNodeAtIndex((i * i + seed) % size));
    }
}

template <class T>
void LinkedList<T>::print(bool reverse) const
{
    if (this->isEmpty())
    {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    if (reverse)
    {
        // traverse in reverse order (last node to first node).

        Node<T> *node = this->getLastNode();

        do
        {
            std::cout << *node << std::endl;
            node = node->prev;
        } while (node != this->getLastNode());
    }
    else
    {
        // traverse in normal order (first node to last node).

        Node<T> *node = this->getFirstNode();

        do
        {
            std::cout << *node << std::endl;
            node = node->next;
        } while (node != this->getFirstNode());
    }
}

template <class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    this->removeAllNodes();
    size = 0;
    if (this != &rhs)
    {
        if (!rhs.head)
        {
            head = NULL;
        }
        else
        {

            head = new Node<T>(rhs.head->data);
            Node<T> *curr = head;
            Node<T> *copy = rhs.head;
            while (copy->next != rhs.head)
            {
                copy = copy->next;
                curr->next = new Node<T>(copy->data, curr, NULL);
                curr = curr->next;
                size++;
            }
            curr->next = head;
            head->prev = curr;
        }
    }
    return *this;
}

#endif //LINKEDLIST_H
