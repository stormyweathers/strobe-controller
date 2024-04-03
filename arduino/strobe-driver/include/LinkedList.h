#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <Arduino.h>

class Node {
public:
    volatile int32_t data;
    Node* next;

    Node(int32_t value);
};

class LinkedList {
public:
    Node* head;

    LinkedList();

    void InsertAtEnd(int32_t value);

    void InsertAtIndex(int32_t value, int16_t index);

    void UpdateNode(int32_t value, int16_t index);

    void PrintList();

    bool IsEquivalent(LinkedList* testList);
};

#endif // LINKED_LIST_H
