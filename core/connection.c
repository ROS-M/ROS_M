#include "connection.h"


char tcpros_init_Buffer_topic_recv[TCPROS_INIT_BUFFER_MAX_SIZE];
extern ros_m_xSemaphoreHandle tcpros_init_Buffer_topic_recv_sem;

char tcpros_Buffer_topic_send_service_server[TCPROS_INIT_BUFFER_MAX_SIZE];
extern ros_m_xSemaphoreHandle tcpros_Buffer_topic_send_service_server_sem;


void ROS_M_CONNECTION_TOPIC_RECV_TASK(void* param)
{
	char tcpros_buffer_topic_recv[TCPROS_BUFFER_MAX_SIZE];
	unsigned long ip = ((ARG*)param) -> ip;
	int xmlrpc_server_port = ((ARG*)param) -> port[0];
	int tcpros_server_port = ((ARG*)param) -> port[1];
	TOPIC* myTopic = ((ARG*)param) -> topic;

	CONNECTION* connection;
	MESSAGE_Q* myMsgQ;

	ros_m_socket_addr_t serveraddr;
	int server_sockfd;

	char* rp;
	int data_len;

	((ARG*)param) -> ip = 0;

	connection = ros_m_topic_add_connection(myTopic, ip, xmlrpc_server_port);
	myMsgQ = connection -> message_q;

	if((server_sockfd = ros_m_lwip_socket()) == -1)
	{
		
		ros_m_vTaskDelete(NULL);
	}

	ros_m_TCP_set_socket_addr(&serveraddr, sizeof(serveraddr), ip, tcpros_server_port);

	if(ros_m_lwip_connect(server_sockfd, &serveraddr, sizeof(serveraddr)) == -1)
	{
		ros_m_lwip_close(server_sockfd);
		
		ros_m_vTaskDelete(NULL);
	}


	ros_m_xSemaphoreTake(tcpros_init_Buffer_topic_recv_sem);

	data_len = ros_m_tcpros_build_topic_tcpros(tcpros_init_Buffer_topic_recv, myTopic);

	if(ros_m_lwip_send(server_sockfd, tcpros_init_Buffer_topic_recv, data_len) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_init_Buffer_topic_recv_sem);
		ros_m_lwip_close(server_sockfd);
		
		ros_m_vTaskDelete(NULL);
	}

	if(ros_m_lwip_recv(server_sockfd, tcpros_init_Buffer_topic_recv, 4) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_init_Buffer_topic_recv_sem);
		
		ros_m_vTaskDelete(NULL);
	}
	data_len = *(int*)tcpros_init_Buffer_topic_recv;

	if(ros_m_lwip_recv(server_sockfd, tcpros_init_Buffer_topic_recv + 4, data_len) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_init_Buffer_topic_recv_sem);
		
		ros_m_vTaskDelete(NULL);
	}

	ros_m_tcpros_parse_tcpros(tcpros_init_Buffer_topic_recv);

	rp = tcpros_init_Buffer_topic_recv;
	while(*rp++ != '\n');
	while(*rp++ != '\n');
	if(strncmp(rp + 7, myTopic -> message_file -> md5sum, 32) != 0)
	{
		ros_m_xSemaphoreGive(tcpros_init_Buffer_topic_recv_sem);
		
		ros_m_vTaskDelete(NULL);
	}


	memset(tcpros_init_Buffer_topic_recv, 0x00, sizeof(tcpros_init_Buffer_topic_recv));

	ros_m_xSemaphoreGive(tcpros_init_Buffer_topic_recv_sem);

	memset(tcpros_buffer_topic_recv, 0x00, sizeof(tcpros_buffer_topic_recv));

	while(ros_m_ok())
	{

		if(ros_m_lwip_recv(server_sockfd, tcpros_buffer_topic_recv, 4) <= 0)
		{
			
			break;
		}
		data_len = *(int*)tcpros_buffer_topic_recv;

		if(data_len > TCPROS_BUFFER_MAX_SIZE - 4)
		{
			
			break;
		}

		if(ros_m_lwip_recv(server_sockfd, tcpros_buffer_topic_recv + 4, data_len) <= 0)
		{
			
			break;
		}

		ros_m_message_enqueue_data(myMsgQ, tcpros_buffer_topic_recv + 4);

		memset(tcpros_buffer_topic_recv, 0x00, data_len + 4);
	}

	ros_m_lwip_close(server_sockfd);

	ros_m_topic_delete_connection(myTopic, ip, xmlrpc_server_port);

	ros_m_vTaskDelete(NULL);
}


void ROS_M_CONNECTION_TOPIC_SEND_SERVICE_SERVER_TASK(void* param)
{
	int client_sockfd = *((int*)param);
	int data_len;
	int name_len = 0;
	char* p;
	static int temp_port = 0;
	int temp_myport = temp_port++;
	int persistance = 0;

	TOPIC* myTopic;
	SERVICE* myService;
	CONNECTION* connection;
	MESSAGE_Q* myMsgQ;

	ros_m_xSemaphoreTake(tcpros_Buffer_topic_send_service_server_sem);

	if(ros_m_lwip_recv(client_sockfd, tcpros_Buffer_topic_send_service_server, 4) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_Buffer_topic_send_service_server_sem);
		
		ros_m_lwip_close(client_sockfd);
		ros_m_vTaskDelete(NULL);
	}

	data_len = *(int*)tcpros_Buffer_topic_send_service_server;

	if(ros_m_lwip_recv(client_sockfd, tcpros_Buffer_topic_send_service_server + 4, data_len) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_Buffer_topic_send_service_server_sem);
		
		ros_m_lwip_close(client_sockfd);
		ros_m_vTaskDelete(NULL);
	}

	ros_m_tcpros_parse_tcpros(tcpros_Buffer_topic_send_service_server);

	p = tcpros_Buffer_topic_send_service_server;
	while(*p++ != '\n');
	while(*p++ != '\n');
	while(*p++ != '\n');

	if(strncmp(p, "topic", 5) == 0)
	{
		char tcpros_Buffer_topic_send[TCPROS_BUFFER_MAX_SIZE];

		while(*(p + 8 + name_len++) != '\n');
		myTopic = ros_m_topic_get_topic(p + 7, name_len, 1);

		p = tcpros_Buffer_topic_send_service_server;

		while(*p++ != '\n');

		if(strncmp(p + 7, myTopic -> message_file -> md5sum, 32) != 0)
		{
			ros_m_xSemaphoreGive(tcpros_Buffer_topic_send_service_server_sem);
			ros_m_lwip_close(client_sockfd);
			
			ros_m_vTaskDelete(NULL);
		}

		data_len = ros_m_tcpros_build_topic_tcpros(tcpros_Buffer_topic_send_service_server, myTopic);

		if(ros_m_lwip_send(client_sockfd, tcpros_Buffer_topic_send_service_server, data_len) <= 0)
		{
			ros_m_xSemaphoreGive(tcpros_Buffer_topic_send_service_server_sem);
			ros_m_lwip_close(client_sockfd);
			
			ros_m_vTaskDelete(NULL);
		}

		memset(tcpros_Buffer_topic_send_service_server, 0x00, sizeof(tcpros_Buffer_topic_send_service_server));

		ros_m_xSemaphoreGive(tcpros_Buffer_topic_send_service_server_sem);

		memset(tcpros_Buffer_topic_send, 0x00, sizeof(tcpros_Buffer_topic_send));

		connection = ros_m_topic_add_connection(myTopic, (unsigned long)0, temp_myport);
		myMsgQ = connection -> message_q;

		while(ros_m_ok())
		{
			if(ros_m_message_dequeue_data(myMsgQ, (void*)tcpros_Buffer_topic_send + 4) < 0)
			{
				ros_m_delay(1);

				continue;
			}

			data_len = ros_m_tcpros_build_message(tcpros_Buffer_topic_send, myMsgQ);
			
			if(data_len > TCPROS_BUFFER_MAX_SIZE - 4)
			{
				

				break;
			}

			if(ros_m_lwip_send(client_sockfd, tcpros_Buffer_topic_send, data_len) < 0)
			{
				

				break;
			}

			memset(tcpros_Buffer_topic_send, 0x00, data_len);
		}

		ros_m_topic_delete_connection(myTopic, (unsigned long)0, temp_myport);
	}
	else if(strncmp(p, "service", 7) == 0)
	{
		char tcpros_Buffer_service_server[TCPROS_BUFFER_MAX_SIZE];
		int pointer = 0;
		int data_len = 0;

		while(*(p + 10 + name_len++) != '\n');
		myService = ros_m_service_get_service(p + 9, name_len);

		p = tcpros_Buffer_topic_send_service_server;

		while(*p++ != '\n');

		if(strncmp(p + 7, myService -> service_file -> md5sum, 32) != 0)
		{
			ros_m_xSemaphoreGive(tcpros_Buffer_topic_send_service_server_sem);
			ros_m_lwip_close(client_sockfd);
			
			ros_m_vTaskDelete(NULL);
		}

		while(*p++ != '\n');
		persistance = atoi(p + 11);

		data_len = ros_m_tcpros_build_service_tcpros(tcpros_Buffer_topic_send_service_server, myService, 1);

		if(ros_m_lwip_send(client_sockfd, tcpros_Buffer_topic_send_service_server, data_len) <= 0)
		{
			ros_m_xSemaphoreGive(tcpros_Buffer_topic_send_service_server_sem);
			ros_m_lwip_close(client_sockfd);
			
			ros_m_vTaskDelete(NULL);
		}

		memset(tcpros_Buffer_topic_send_service_server, 0x00, sizeof(tcpros_Buffer_topic_send_service_server));

		ros_m_xSemaphoreGive(tcpros_Buffer_topic_send_service_server_sem);

		memset(tcpros_Buffer_service_server, 0x00, sizeof(tcpros_Buffer_service_server));


		while(ros_m_ok())
		{
			if(ros_m_lwip_recv(client_sockfd, tcpros_Buffer_service_server, 4) <= 0)
			{
				ros_m_lwip_close(client_sockfd);
				
				ros_m_vTaskDelete(NULL);
			}

			data_len = *(int*)tcpros_Buffer_service_server;

			if(data_len > TCPROS_BUFFER_MAX_SIZE - 4)
			{
				

				break;
			}
		
			if(ros_m_lwip_recv(client_sockfd, tcpros_Buffer_service_server, data_len) <= 0)
			{
				ros_m_lwip_close(client_sockfd);
				
				ros_m_vTaskDelete(NULL);
			}

			while(pointer < data_len)
				*(char*)(myService -> data + pointer) = *(char*)(tcpros_Buffer_service_server + pointer++);

			(*(myService -> callback))(myService -> data);

			*tcpros_Buffer_service_server = 1;
			*(int*)(tcpros_Buffer_service_server + 1) = myService -> response_data_size;
		
			pointer = 0;
			while(pointer < myService -> response_data_size)
				*(char*)(tcpros_Buffer_service_server + 5 + pointer) = *(char*)(myService -> data + myService -> request_data_size + pointer++);
		
			if(ros_m_lwip_send(client_sockfd, tcpros_Buffer_service_server, myService -> response_data_size + 5) <= 0)
			{
				ros_m_lwip_close(client_sockfd);
				
				ros_m_vTaskDelete(NULL);
			}

			if(persistance == 0) break;
		}
	}


	ros_m_lwip_close(client_sockfd);

	ros_m_vTaskDelete(NULL);
}


