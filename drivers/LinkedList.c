#include <stdlib.h>
#include "LinkedList.h"

typedef struct Node {
	int block_type;
	struct Node *next;
} Node;

Node *head = NULL; // Initialize head and tail to NULL
Node *tail = NULL;

void fifo_append(int block_type) {
	Node *newNode = malloc(sizeof(Node));
	newNode->block_type = block_type;
	newNode->next = NULL;

	if (tail != NULL) {       // If list not empty
		tail->next = newNode;
		tail = newNode;
		} 
	else {                    // If list is empty
		head = newNode;
		tail = newNode;
	}
	return;
}

void fifo_pop_front() {
	if (head == NULL){ // If list is empty
		return;
	}
	Node *temp = head;
	head = head->next;
	if (head == NULL) { // If last item in list
		tail = NULL;    // Also set tail to NULL
	}
	free(temp);         // Free memory
	return;
}

int get_next_block_type() {
	if (head == NULL) {
		return -1; // Empty indicator
	}
	return head->block_type;
}

int is_fifo_empty(){
	return (head == NULL);
}

void clear_fifo() {
	while (!is_fifo_empty()){ // Pop front until fifo is empty
		fifo_pop_front();
	}
}