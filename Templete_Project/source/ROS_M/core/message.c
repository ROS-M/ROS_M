#include "message.h"


MSG_FILE* ros_m_message_create_msg_file(char* name, char* definition, int byte, char* md5sum)
{
	MSG_FILE* msg = (MSG_FILE*)ros_m_pvPortMalloc(sizeof(MSG_FILE));

	msg -> name = name;
	msg -> definition = definition;
	msg -> data_size = byte;
	msg -> md5sum = md5sum;

	return msg;
}


MESSAGE_Q* ros_m_message_create_message_queue(int maxElements)
{
	MESSAGE_Q* Q = (MESSAGE_Q*)ros_m_pvPortMalloc(sizeof(MESSAGE_Q));

	Q -> capacity = maxElements + QUEUE_IDLE_SIZE;
	Q -> element_byte = 0;
	makeEmpty(Q);

	return Q;
}


int ros_m_message_enqueue_data(MESSAGE_Q* Q, void* msg_struct)
{
	int pointer = 0;

	if(Q == 0)
		return -1;
	else if(Q -> element_byte == 0)
		return -1;
	else
	{
		if(isFull(Q))
		{
			Q -> size--;
			Q -> front = succ(Q -> front, Q);
		}

		Q -> rear = succ(Q -> rear, Q);
		while(pointer < Q -> element_byte)
			*(char*)(Q -> array + Q -> element_byte * Q -> rear + pointer) = *(char*)(msg_struct + pointer++);

		Q -> size++;
	}

	return 1;
}


int ros_m_message_dequeue_data(MESSAGE_Q* Q, void* msg_struct)
{
	int pointer = 0;

	if(Q -> element_byte == 0)
	{
		return -1;
	}
	else if(isEmpty(Q))
	{
		return -1;
	}
	else
	{
		Q -> front = succ(Q -> front, Q);
		while(pointer < Q -> element_byte)
			*(char*)(msg_struct + pointer) = *(char*)(Q -> array + Q -> element_byte * Q -> front + pointer++);

		Q -> size--;
	}
	return 1;
}

int succ(int value, MESSAGE_Q* Q)
{
	if(++value == Q -> capacity)
		value = 0;

	return value;
}

int isEmpty(MESSAGE_Q* Q)
{
	return Q -> size == 0;
}

int isFull(MESSAGE_Q* Q)
{
	return Q -> size == Q -> capacity - QUEUE_IDLE_SIZE;
}

void makeEmpty(MESSAGE_Q* Q)
{
	Q -> size = 0;
	Q -> front = 1;
	Q -> rear = 1;
}


