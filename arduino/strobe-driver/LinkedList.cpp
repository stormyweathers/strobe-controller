#include "LinkedList.h"

Node::Node(int value) : data(value), next(nullptr) {}

LinkedList::LinkedList() : head(nullptr) {}

void LinkedList::InsertAtEnd(int value) {
    Node* newNode = new Node(value);
    if (!this->head) {
        this->head = newNode;
    } else {
        Node* current = this->head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void LinkedList::InsertAtIndex(int value, int index) {
    Node* newNode = new Node(value);
    if (index == 0) {
        newNode->next = this->head;
        this->head = newNode;
    } else {
        Node* current = this->head;
        for (int i = 0; i < index - 1 && current; ++i) {
            current = current->next;
        }
        if (current) {
            newNode->next = current->next;
            current->next = newNode;
        }
    }
}

void LinkedList::UpdateNode(int value, int index) {
    Node* current = this->head;
    for (int i = 0; i < index && current; ++i) {
        current = current->next;
    }
    if (current) {
        current->data = value;
    }
}