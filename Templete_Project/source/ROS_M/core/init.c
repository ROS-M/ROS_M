#include "init.h"


extern LINKED_LIST* topic_Linked_list;
extern int xmlrpc_Server_sockfd;
extern int tcpros_Server_sockfd;

char node_Name[NODE_NAME_MAX_LENGTH + 1];
void* callback_Param;
int ros_m_Ok;

ros_m_xSemaphoreHandle xmlrpc_Buffer_call_sem;
ros_m_xSemaphoreHandle tcpros_init_Buffer_topic_recv_sem;
ros_m_xSemaphoreHandle tcpros_Buffer_service_call_sem;
ros_m_xSemaphoreHandle tcpros_Buffer_topic_send_service_server_sem;


int ros_m_init(int argc, char** argv, char* node_name)
{
	int pointer = 0;

	while(*(node_name + pointer) != 0)
	{
		*(node_Name + pointer) = *(node_name + pointer);
		pointer++;
		if(pointer >= NODE_NAME_MAX_LENGTH)
			break;
	}
	*(node_Name + pointer) = 0;

	callback_Param = ros_m_pvPortMalloc(CALLBACK_PARAM_MAX_SIZE);

	if(ros_m_xSemaphoreCreateCounting(&xmlrpc_Buffer_call_sem, 1) < 0) return -1;
	if(ros_m_xSemaphoreCreateCounting(&tcpros_init_Buffer_topic_recv_sem, 1) < 0) return -1;
	if(ros_m_xSemaphoreCreateCounting(&tcpros_Buffer_service_call_sem, 1) < 0) return -1;
	if(ros_m_xSemaphoreCreateCounting(&tcpros_Buffer_topic_send_service_server_sem, 1) < 0) return -1;

	ros_m_Ok = 1;

	return 1;
}

char* ros_m_get_node_name()
{
	return node_Name;
}

int ros_m_ok()
{
	return ros_m_Ok;
}

void ros_m_shutdown()
{
	ros_m_Ok = 0;
	ros_m_topic_delete_all_topic();
	ros_m_service_delete_all_service();
	ros_m_lwip_close(xmlrpc_Server_sockfd);
	ros_m_lwip_close(tcpros_Server_sockfd);
	ros_m_vSemaphoreDelete(&xmlrpc_Buffer_call_sem);
	ros_m_vSemaphoreDelete(&tcpros_init_Buffer_topic_recv_sem);
	ros_m_vSemaphoreDelete(&tcpros_Buffer_service_call_sem);
	ros_m_vSemaphoreDelete(&tcpros_Buffer_topic_send_service_server_sem);
	ros_m_vPortFree(callback_Param);
}


void ros_m_spin()
{
	ros_m_tick_t xLastWakeTime;
	const ros_m_tick_t xFrequency = 1;
	xLastWakeTime = ros_m_get_tickCount();

	while(ros_m_ok())
	{
		ros_m_call_callbacks(callback_Param);

		ros_m_delayUntil(&xLastWakeTime, xFrequency);
	}
}

void ros_m_spinOnce()
{
	ros_m_call_callbacks(callback_Param);
}

void ros_m_call_callbacks(void* callback_Param)
{
	NODE* topic_node;
	NODE* connection_node;
	TOPIC* topic;
	CONNECTION* connection;

	topic_node = topic_Linked_list -> head;

	while(topic_node -> next != NULL)
	{
		topic = (TOPIC*)(topic_node -> next -> data);
		if(topic -> isPublisher == 0)
		{
			if(topic -> connection_linked_list -> node_num > 1)
			{
				connection_node = topic -> connection_linked_list -> head;
				while(connection_node -> next != NULL)
				{
					connection = (CONNECTION*)(connection_node -> next -> data);
					if(connection != NULL && connection -> message_q -> size > 0)
					{
						if(ros_m_message_dequeue_data(connection -> message_q, callback_Param) > 0)
						{
							(*(topic -> callback))(callback_Param);
						}
					}
					connection_node = connection_node -> next;
				}
			}
		}
		topic_node = topic_node -> next;
	}
}



