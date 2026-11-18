#ifndef LINKED_LIST_H
#define LINKED_LIST_H

void fifo_append(int block_type);
int  get_next_block_type(void);
void fifo_pop_front(void);
int is_fifo_empty(void);
void clear_fifo(void);
unsigned int get_num_sorted(int block_type);
unsigned int get_num_partially_sorted();

#endif