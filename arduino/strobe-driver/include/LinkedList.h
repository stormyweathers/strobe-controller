#ifndef LINKED_LIST_H
#define LINKED_LIST_H

class Node {
public:
    int data;
    Node* next;

    Node(int value);
};

class LinkedList {
public:
    Node* head;

    LinkedList();

    void InsertAtEnd(int value);

    void InsertAtIndex(int value, int index);

    void UpdateNode(int value, int index);

    void PrintList();

    bool IsEquivalent(LinkedList* testList);
};

#endif // LINKED_LIST_H
