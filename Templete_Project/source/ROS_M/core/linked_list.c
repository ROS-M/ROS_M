#include "linked_list.h"


LINKED_LIST* create_linked_list()
{
	LINKED_LIST* linked_list = (LINKED_LIST*)ros_m_pvPortMalloc(sizeof(LINKED_LIST));
	linked_list -> node_num = 0;
	linked_list -> tail = 0;
	linked_list -> head = create_node(linked_list);

	return linked_list;
}

NODE* create_node(LINKED_LIST* list)
{
	NODE* node = (NODE*)ros_m_pvPortMalloc(sizeof(NODE));
	if(list -> tail != (void*)0) list -> tail -> next = node;
	list -> tail = node;
	list -> node_num++;
	node -> next = (void*)0;
	node -> data = (void*)0;
	
	return node;
}
