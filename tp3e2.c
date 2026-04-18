1.The header file:

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Structure representing a node in the linked list.
 */
struct Node {
    int data;
    struct Node* next;
};

// Function prototypes
struct Node* createNode(int data);
void addNode(struct Node** head, int data);
void addNodeAtBeginning(struct Node** head, int data);
void addNodeAfter(struct Node* prevNode, int data);
void displayList(struct Node* head);
int search(struct Node* head, int key);

#endif

2.The implementation file:


#include "list.h"

/**
 * Creates a new node with the given data.
 * Returns a pointer to the newly allocated node.
 */
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

/**
 * Adds a new node to the end of the linked list.
 */
void addNode(struct Node** head, int data) {
    struct Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    struct Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

/**
 * Adds a new node to the beginning of the linked list.
 */
void addNodeAtBeginning(struct Node** head, int data) {
    struct Node* newNode = createNode(data);
    if (newNode) {
        newNode->next = *head;
        *head = newNode;
    }
}

/**
 * Adds a new node after the specified node.
 */
void addNodeAfter(struct Node* prevNode, int data) {
    if (prevNode == NULL) {
        printf("Error: The given previous node cannot be NULL.\n");
        return;
    }
    struct Node* newNode = createNode(data);
    if (newNode) {
        newNode->next = prevNode->next;
        prevNode->next = newNode;
    }
}

/**
 * Displays the contents of the linked list.
 */
void displayList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

/**
 * Searches for a key in the list. Returns 1 if found, 0 otherwise.
 */
int search(struct Node* head, int key) {
    struct Node* temp = head;
    while (temp != NULL) {
        if (temp->data == key) return 1;
        temp = temp->next;
    }
    return 0;
}
