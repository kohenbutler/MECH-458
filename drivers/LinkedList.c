#include <stdlib.h>
#include "LinkedList.h"

typedef struct Node {
	int block_type;
	struct Node *next;
} Node;

Node *head = NULL; // Initialize head and tail to NULL
Node *tail = NULL;

volatile unsigned int num_partially_sorted = 0;
volatile unsigned int num_sorted[4] = {0, 0, 0, 0};

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
	num_partially_sorted += 1;
	return;
}

int get_next_block_type() {
	if (head == NULL) {
		return -1; // Empty indicator
	}
	return head->block_type;
}

void fifo_pop_front() {
	if (head == NULL){ // If list is empty
		return;
	}
	num_partially_sorted -= 1;                    // reduce count of partially sorted pieces
	num_sorted[get_next_block_type()] += 1;      // increase count of sorted pieces
	Node *temp = head;
	head = head->next;
	if (head == NULL) { // If last item in list
		tail = NULL;    // Also set tail to NULL
	}
	free(temp);         // Free memory
	return;
}

int is_fifo_empty(){
	return (head == NULL);
}

void clear_fifo() {
	while (!is_fifo_empty()){ // Pop front until fifo is empty
		fifo_pop_front();
	}
}

unsigned int get_num_sorted(int block_type){
	return num_sorted[block_type];
}

unsigned int get_num_partially_sorted(){
	return num_partially_sorted;
}