#ifndef LINKED_LIST_H
#define LINKED_LIST_H

void fifo_append(int block_type);
void fifo_pop_front(void);
int  get_next_block_type(void);
int is_fifo_empty(void);
void clear_fifo(void);

#endif