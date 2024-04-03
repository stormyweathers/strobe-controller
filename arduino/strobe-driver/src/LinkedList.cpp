#include "LinkedList.h"
#include <Arduino.h>
Node::Node(int32_t value) : data(value), next(nullptr) {}

LinkedList::LinkedList() : head(nullptr) {}

void LinkedList::InsertAtEnd(int32_t value) {
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

void LinkedList::InsertAtIndex(int32_t value, int16_t index) {
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

void LinkedList::UpdateNode(int32_t value, int16_t index) {
    Node* current = this->head;
    for (int i = 0; i < index && current; ++i) {
        current = current->next;
    }
    if (current) {
        current->data = value;
    }
}

void LinkedList::PrintList(){
    Node* current_node = this->head;
    while( nullptr != current_node )
    {
        Serial.print( current_node->data);
        Serial.print(" -> ");
        current_node = current_node->next;
    }
    Serial.println( "(nullptr)");
}

bool LinkedList::IsEquivalent(LinkedList* testList){
    Node* current = this->head;
    Node* test_current = testList->head;
    while (nullptr != current)
    {
        if ( current->data != test_current->data)
        {
            return false;
        }
        current = current->next;
        test_current = test_current->next;
    }
    return true;

}
