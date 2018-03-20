#include "topic.h"


LINKED_LIST* topic_Linked_list;


void ros_m_topic_init()
{
	topic_Linked_list = create_linked_list();
}


TOPIC* ros_m_topic_add_topic(MSG_FILE* message_file, char* topic_name, int queue_size, int latch, int isPublisher)
{
	int byte = 0;
	int pointer = 0;
	NODE* node;
	TOPIC* topic;

	node = create_node(topic_Linked_list);
	node -> data = (void*)ros_m_pvPortMalloc(sizeof(TOPIC));
	topic = (TOPIC*)(node -> data);

	topic -> isPublisher = isPublisher;
	topic -> name = topic_name;
	topic -> queue_size = queue_size;
	topic -> latch = latch;

	topic -> message_file = message_file;

	topic -> connection_linked_list = create_linked_list();

	return topic;
}


TOPIC* ros_m_topic_get_topic(char* topic_name, int name_len, int isPublisher)
{
	TOPIC* topic;
	NODE* node = topic_Linked_list -> head;

	while(node -> next != 0)
	{
		topic = (TOPIC*)(node -> next -> data);
		if(strncmp(topic_name, topic -> name, name_len) == 0 && strlen(topic -> name) == name_len && topic -> isPublisher == isPublisher)
		{
			return topic;
		}
		node = node -> next;
	}

	return 0;
}


void ros_m_topic_delete_topic(char* topic_name, int name_len)
{
	TOPIC* topic;
	NODE* node = topic_Linked_list -> head;

	while(node -> next != 0)
	{
		if(strncmp(topic_name, topic -> name, name_len) == 0 && strlen(topic -> name) == name_len)
		{
			if(node -> next == topic_Linked_list -> tail)
				topic_Linked_list -> tail = node;
			ros_m_vPortFree(node -> next);
			node -> next = node -> next -> next;

			break;
		}
		node = node -> next;
	}
}


CONNECTION* ros_m_topic_add_connection(TOPIC* topic, unsigned long ip, int port)
{
	CONNECTION* connection;
	NODE* node = create_node(topic -> connection_linked_list);

	node -> data = (void*)ros_m_pvPortMalloc(sizeof(CONNECTION));
	connection = (CONNECTION*)(node -> data);
	
	connection -> ip = ip;
	connection -> port = port;

	connection -> message_q = (MESSAGE_Q*)ros_m_message_create_message_queue(topic -> queue_size);
	connection -> message_q -> element_byte = topic -> message_file -> data_size;
	connection -> message_q -> array = (void*)ros_m_pvPortMalloc(topic -> message_file -> data_size * (topic -> queue_size + QUEUE_IDLE_SIZE));

	return connection;
}


CONNECTION* ros_m_topic_find_connection(TOPIC* topic, unsigned long ip, int port)
{
	CONNECTION* connection;
	NODE* node = topic -> connection_linked_list -> head;

	while(node -> next != 0)
	{
		connection = (CONNECTION*)(node -> next -> data);
		if(ip == connection -> ip && port == connection -> port)
		{
			return connection;
		}
		node = node -> next;
	}

	return 0;
}


void ros_m_topic_delete_connection(TOPIC* topic, unsigned long ip, int port)
{
	CONNECTION* connection;
	NODE* node = topic -> connection_linked_list -> head;

	while(node -> next != 0)
	{
		connection = (CONNECTION*)(node -> next -> data);
		if(ip == connection -> ip && port == connection -> port)
		{
			if(node -> next == topic -> connection_linked_list -> tail)
				topic -> connection_linked_list -> tail = node;
			ros_m_vPortFree(connection -> message_q -> array);
			ros_m_vPortFree(connection -> message_q);
			ros_m_vPortFree(connection);
			ros_m_vPortFree(node -> next);
			node -> next = node -> next -> next;
			
			break;
		}
		node = node -> next;
	}
}


void ros_m_topic_delete_all_topic()
{
	TOPIC* topic;
	NODE* topic_node = topic_Linked_list -> head;

	while(topic_node -> next != 0)
	{
		topic = (TOPIC*)(topic_node -> next -> data);
		if(topic -> isPublisher == 1)
			ros_m_xmlrpc_call_unregisterPublisher(topic, NULL, 0);
		else
			ros_m_xmlrpc_call_unregisterSubscriber(topic, NULL, 0);
		topic_node = topic_node -> next;
	}
}


