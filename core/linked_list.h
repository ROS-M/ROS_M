#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include "ros_m_opts.h"
#include "ros_m_portable.h"


typedef struct NODE
{
	void* data;
	struct NODE* next;
} NODE;


typedef struct LINKED_LIST
{
	int node_num;
	NODE* head;
	NODE* tail;
} LINKED_LIST;


LINKED_LIST* create_linked_list(void);

NODE* create_node(LINKED_LIST* list);


#endif
