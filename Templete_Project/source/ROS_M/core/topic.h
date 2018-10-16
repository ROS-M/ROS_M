#ifndef TOPIC_H
#define TOPIC_H


#include "ros_m_opts.h"
#include "ros_m_portable.h"
#include "stdlib.h"
#include "message.h"
#include "linked_list.h"


typedef struct TOPIC_CONNECTION
{
	unsigned long ip;
	int port;
	MESSAGE_Q* message_q;
} CONNECTION;


typedef struct TOPIC_PROPERTIES
{
	char* name;
	int isPublisher;
	int queue_size;
	int latch;
	void (*callback)(void* param);
	LINKED_LIST* connection_linked_list;
	MSG_FILE* message_file;
} TOPIC;


#include "xmlrpc.h"


void ros_m_topic_init(void);

TOPIC* ros_m_topic_add_topic(MSG_FILE* message_file, char* topic_name, int queue_size, int latch, int isPublisher);

TOPIC* ros_m_topic_get_topic(char* topic_name, int name_len, int isPublisher);

void ros_m_topic_delete_topic(char* topic_name, int name_len);

CONNECTION* ros_m_topic_add_connection(TOPIC* topic, unsigned long ip, int port);

CONNECTION* ros_m_topic_find_connection(TOPIC* topic, unsigned long ip, int port);

void ros_m_topic_delete_connection(TOPIC* topic, unsigned long ip, int port);

void ros_m_topic_delete_all_topic(void);


#endif
