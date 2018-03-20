#include "server.h"


char xmlrpc_Buffer_server[XMLRPC_BUFFER_MAX_SIZE];
int xmlrpc_Server_sockfd;

int tcpros_Server_sockfd;


void ROS_M_SERVER_XMLRPC_TASK(void* param)
{
	int client_sockfd;
	int opt = 1;
	struct sockaddr_in clientaddr, serveraddr;

	if((xmlrpc_Server_sockfd = ros_m_lwip_socket()) == -1)
		;

	ros_m_TCP_set_socket_addr(&serveraddr, sizeof(serveraddr), htonl(INADDR_ANY), ROS_HOST_XMLRPC_PORT);

	struct linger l;
	l.l_onoff = 1;
	l.l_linger = 0;
	if(ros_m_setsockopt(xmlrpc_Server_sockfd, SOL_SOCKET, SO_LINGER, (char*)&l, sizeof(l)) < 0)
	{
		;
	}

	if (ros_m_setsockopt(xmlrpc_Server_sockfd, IPPROTO_TCP, TCP_NODELAY, (char*) &opt, sizeof(opt)) < 0)
	{
		;
	}

	ros_m_lwip_bind(xmlrpc_Server_sockfd, &serveraddr, sizeof(serveraddr));

	ros_m_lwip_listen(xmlrpc_Server_sockfd);

	while(ros_m_ok())
	{
		memset(xmlrpc_Buffer_server, 0x00, XMLRPC_BUFFER_MAX_SIZE);

		if((client_sockfd = ros_m_lwip_accept(xmlrpc_Server_sockfd, &clientaddr, sizeof(clientaddr))) == -1)
		{
			continue;
		}

		if(ros_m_lwip_recv(client_sockfd, xmlrpc_Buffer_server, XMLRPC_BUFFER_MAX_SIZE) <= 0)
		{
			ros_m_lwip_close(client_sockfd);

			continue;
		}

		if(ros_m_http_parse_header(xmlrpc_Buffer_server, XMLRPC_BUFFER_MAX_SIZE) == -1)
			;

		if(ros_m_xmlrpc_parse_xmlrpc(xmlrpc_Buffer_server, XMLRPC_BUFFER_MAX_SIZE, xmlrpc_Buffer_server, XMLRPC_BUFFER_MAX_SIZE) < 0)
			;


		if(strncmp(xmlrpc_Buffer_server, "requestTopic", 12) == 0)
		{
			ros_m_xmlrpc_api_requestTopic(xmlrpc_Buffer_server, XMLRPC_BUFFER_MAX_SIZE, client_sockfd);
		}
		else if(strncmp(xmlrpc_Buffer_server, "publisherUpdate", 15) == 0)
		{
			ros_m_xmlrpc_api_publisherUpdate(xmlrpc_Buffer_server, XMLRPC_BUFFER_MAX_SIZE, client_sockfd);
		}
		else if(strncmp(xmlrpc_Buffer_server, "shutdown", 8) == 0)
		{
			ros_m_xmlrpc_api_shutdown();
		}
		
		ros_m_lwip_close(client_sockfd);
	}

	ros_m_vTaskDelete(NULL);
}

void ROS_M_SERVER_TCPROS_TASK(void *param)
{
	int client_sockfd;
	int opt = 1;
	struct sockaddr_in clientaddr, serveraddr;

	if((tcpros_Server_sockfd = ros_m_lwip_socket()) == -1)
		;

	ros_m_TCP_set_socket_addr(&serveraddr, sizeof(serveraddr), htonl(INADDR_ANY), ROS_HOST_TCPROS_PORT);

	struct linger l;
	l.l_onoff = 1;
	l.l_linger = 0;
	if(ros_m_setsockopt(tcpros_Server_sockfd, SOL_SOCKET, SO_LINGER, (char*)&l, sizeof(l)) < 0)
	{
		;
	}

	if(ros_m_setsockopt(tcpros_Server_sockfd, IPPROTO_TCP, TCP_NODELAY, (char*) &opt, sizeof(opt)) < 0)
	{
		;
	}

	ros_m_lwip_bind(tcpros_Server_sockfd, &serveraddr, sizeof(serveraddr));

	ros_m_lwip_listen(tcpros_Server_sockfd);

	while(ros_m_ok())
	{
		if((client_sockfd = ros_m_lwip_accept(tcpros_Server_sockfd, &clientaddr, sizeof(clientaddr))) == -1)
		{
			continue;
		}

		if(ros_m_xTaskCreate(ROS_M_CONNECTION_TOPIC_SEND_SERVICE_SERVER_TASK, "ROS_M_CONNECTION_TOPIC_SEND_SERVICE_SERVER_TASK", TOPIC_SEND_SERVICE_SERVER_CONNECTION_TASK_STACK_SIZE, (void*)&client_sockfd, ROS_M_CONNECTION_TOPIC_SEND_SERVICE_SERVER_TASK_PRIORITY, NULL) < 0)
			;
	}

	ros_m_vTaskDelete(NULL);
}


