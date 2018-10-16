#include "service.h"


LINKED_LIST* service_Linked_list;

char tcpros_Buffer_service_call[TCPROS_INIT_BUFFER_MAX_SIZE];
extern ros_m_xSemaphoreHandle tcpros_Buffer_service_call_sem;


void ros_m_service_init()
{
	service_Linked_list = create_linked_list();
}


SERVICE_FILE* ros_m_service_create_service_file(char* name, char* md5sum, char* definition)
{
	SERVICE_FILE* srv = (SERVICE_FILE*)ros_m_pvPortMalloc(sizeof(SERVICE_FILE));

	srv -> name = name;
	srv -> md5sum = md5sum;
	srv -> definition = definition;

	return srv;
}


SERVICE* ros_m_service_add_service(SERVICE_FILE* service_file, char* service_name, void (*callback)(void* param))
{
	NODE* node;
	SERVICE* service;
	int pointer = 0;
	int byte = 0;

	node = create_node(service_Linked_list);
	node -> data = (void*)ros_m_pvPortMalloc(sizeof(SERVICE));
	service = (SERVICE*)(node -> data);

	service -> name = service_name;

	service -> service_file = service_file;
	service -> callback = callback;

	while(*((service_file -> definition) + pointer) != 0)
	{
		if(strncmp((service_file -> definition) + pointer, "int", 3) == 0)
		{
			byte += atoi((service_file -> definition) + pointer + 3) / 8;
		}
		else if(strncmp((service_file -> definition) + pointer, "uint", 4) == 0)
		{
			byte += atoi((service_file -> definition) + pointer + 4) / 8;
		}
		else if(strncmp((service_file -> definition) + pointer, "float", 5) == 0)
		{
			byte += atoi((service_file -> definition) + pointer + 5) / 8;
		}
		else if(strncmp((service_file -> definition) + pointer, "---", 3) == 0)
		{
			service -> request_data_size = byte;
			byte = 0;
		}
		while(*((service_file -> definition) + pointer++) != '\n');
	}
	service -> response_data_size = byte;

	service -> data = (void*)ros_m_pvPortMalloc(service -> request_data_size + service -> response_data_size);

	return service;
}


SERVICE* ros_m_service_get_service(char* service_name, int name_len)
{
	SERVICE* service;
	NODE* node = service_Linked_list -> head;

	while(node -> next != NULL)
	{
		service = (SERVICE*)(node -> next -> data);
		if(strncmp(service_name, service -> name, name_len) == 0 && strlen(service -> name) == name_len)
		{
			return service;
		}
		node = node -> next;
	}

	return NULL;
}


int ros_m_service_call(char* service_name, void* srv_struct)
{	
	struct sockaddr_in serveraddr;
	int server_sockfd;
	char res[128];
	char* rp = res;
	SERVICE* myService;
	char* ip;
	int port;
	int pointer = 0;
	int data_len = 0;

	myService = ros_m_service_get_service(service_name, strlen(service_name));
	if(myService == NULL)
		return -1;

	if(myService -> persistance == 0)
	{
		ros_m_xmlrpc_call_lookupService(service_name, res, sizeof(res));

		if(atoi(rp) == -1)
		{
			return -1;
		}

		while(*rp++ != '\n');
		while(*rp++ != '\n');
		ip = rp + 9;
		while(*rp++ != ':');
		while(*rp++ != ':');

		port = atoi(rp);
		*(rp - 1) = 0;
	}

	if((server_sockfd = ros_m_lwip_socket()) == -1)
	{
		return -1;
	}

	if(myService -> persistance == 0)
		ros_m_TCP_set_socket_addr(&serveraddr, sizeof(serveraddr), inet_addr(ip), port);
	else if(myService -> persistance == 1)
		ros_m_TCP_set_socket_addr(&serveraddr, sizeof(serveraddr), myService -> ip, myService -> port);

	if(ros_m_lwip_connect(server_sockfd, &serveraddr, sizeof(serveraddr)) == -1)
	{
		ros_m_lwip_close(server_sockfd);

		return -1;
	}

	ros_m_xSemaphoreTake(tcpros_Buffer_service_call_sem);
	data_len = ros_m_tcpros_build_service_tcpros(tcpros_Buffer_service_call, myService, 0);

	if(ros_m_lwip_send(server_sockfd, tcpros_Buffer_service_call, data_len) <= 0)
	{
		ros_m_lwip_close(server_sockfd);

		return -1;
	}
	
	if(ros_m_lwip_recv(server_sockfd, tcpros_Buffer_service_call, sizeof(tcpros_Buffer_service_call)) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_Buffer_service_call_sem);
		ros_m_lwip_close(server_sockfd);

		return -1;
	}

	*(int*)tcpros_Buffer_service_call = myService -> request_data_size;
	while(pointer < myService -> request_data_size)
		*(char*)(tcpros_Buffer_service_call + 4 + pointer) = *(char*)(srv_struct + pointer++);

	if(ros_m_lwip_send(server_sockfd, tcpros_Buffer_service_call, myService -> request_data_size + 4) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_Buffer_service_call_sem);
		ros_m_lwip_close(server_sockfd);

		return -1;
	}

	if(ros_m_lwip_recv(server_sockfd, tcpros_Buffer_service_call, 1) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_Buffer_service_call_sem);
		ros_m_lwip_close(server_sockfd);

		return -1;
	}

	if(*tcpros_Buffer_service_call == -1)
	{
		ros_m_xSemaphoreGive(tcpros_Buffer_service_call_sem);

		return -1;
	}

	if(ros_m_lwip_recv(server_sockfd, tcpros_Buffer_service_call, 4) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_Buffer_service_call_sem);
		ros_m_lwip_close(server_sockfd);

		return -1;
	}

	data_len = *(int*)tcpros_Buffer_service_call;

	if(ros_m_lwip_recv(server_sockfd, tcpros_Buffer_service_call, data_len) <= 0)
	{
		ros_m_xSemaphoreGive(tcpros_Buffer_service_call_sem);
		ros_m_lwip_close(server_sockfd);

		return -1;
	}

	pointer = 0;
	while(pointer < data_len)
		*(char*)(srv_struct + myService -> request_data_size + pointer) = *(char*)(tcpros_Buffer_service_call + pointer++);

	ros_m_xSemaphoreGive(tcpros_Buffer_service_call_sem);

	ros_m_lwip_close(server_sockfd);

	return 1;
}


void ros_m_service_delete_all_service()
{
	SERVICE* service;
	NODE* service_node = service_Linked_list -> head;

	while(service_node -> next != NULL)
	{
		service = (SERVICE*)(service_node -> next -> data);
		ros_m_xmlrpc_call_unregisterService(service, NULL, 0);

		service_node = service_node -> next;
	}
}




