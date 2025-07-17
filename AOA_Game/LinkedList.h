#pragma once
#include <iostream>
using namespace std;

// Node structure
struct Node {
    int** data;
    Node* next;

    Node(int** val) {
        data = val;
        next = nullptr;
    }
};

// Stack class using linked list
class Stack {
private:
    Node* topNode;

public:
    Stack() {
        topNode = nullptr;
    }

    // Push element onto the stack
    void push(int** val) {
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
    }

    // Pop the top element
    int** pop() {
        if (isEmpty()) {
            cout << "Stack Underflow!\n";
           
        }
        Node* temp = topNode;
        topNode = topNode->next;
        return temp->data;
        delete temp;
    }

    // Get the top element
    int** top() {
        if (isEmpty()) {
            cout << "Stack is empty!\n";
            // or throw an exception
        }
        return topNode->data;
    }

    // Check if stack is empty
    bool isEmpty() {
        return topNode == nullptr;
    }

    // Destructor to free memory
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
};