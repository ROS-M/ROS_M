#ifndef MESSAGE_H
#define MESSAGE_H

#include "ros_m_opts.h"
#include "ros_m_portable.h"


#ifndef QUEUE_IDLE_SIZE
#define QUEUE_IDLE_SIZE 1
#endif


//==================================
#pragma pack(push)
#pragma pack(1)

#include "msg_file.h"

#pragma pack(pop)
//==================================


typedef struct MESSAGE_FILE
{
	char* name;
	char* md5sum;
	char* definition;
	int data_size;
} MSG_FILE;


typedef struct MESSAGE_QUEUE
{
	int capacity;
	int size;
	int front;
	int rear;
	int element_byte;
	void* array;
} MESSAGE_Q;


MSG_FILE* ros_m_message_create_msg_file(char* name, char* definition, int byte, char* md5sum);

MESSAGE_Q* ros_m_message_create_message_queue(int maxElements);

int ros_m_message_enqueue_data(MESSAGE_Q* Q, void* msg_struct);

int ros_m_message_dequeue_data(MESSAGE_Q* Q, void* msg_struct);

int succ(int value, MESSAGE_Q* Q);

int isEmpty(MESSAGE_Q* Q);

int isFull(MESSAGE_Q* Q);

void makeEmpty(MESSAGE_Q* Q);


#endif
