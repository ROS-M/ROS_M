#include "node_handle.h"


ARG _arg;
ARG* arg = &_arg;


int ros_m_nodeHandle_init()
{
	char service_name1[NODE_NAME_MAX_LENGTH + 13];
	char service_name2[NODE_NAME_MAX_LENGTH + 18];
	char* node_name = ros_m_get_node_name();

	ros_m_topic_init();
	ros_m_service_init();

	if(ros_m_xTaskCreate(ROS_M_SERVER_XMLRPC_TASK, "ROS_M_SERVER_XMLRPC_TASK", XMLRPC_SERVER_TASK_STACK_SIZE, NULL, ROS_M_SERVER_XMLRPC_TASK_PRIORITY, NULL) < 0)
		return -1;

	if(ros_m_xTaskCreate(ROS_M_SERVER_TCPROS_TASK, "ROS_M_SERVER_TCPROS_TASK", TCPROS_SERVER_TASK_STACK_SIZE, NULL, ROS_M_SERVER_TCPROS_TASK_PRIORITY, NULL) < 0)
		return -1;

	sprintf(service_name1, "%s/get_loggers", node_name);
	sprintf(service_name2, "%s/set_logger_level", node_name);

	return 1;
}


int ros_m_nodeHandle_hasParam_str(char* param)
{
	char res[64];
	char* rp = res;

	ros_m_xmlrpc_call_hasParam(param, res, sizeof(res));

	while(*rp++ != '\n');
	while(*rp++ != '\n');

	if(*rp == '1') return 1;
	else return 0;
}


void ros_m_nodeHandle_setParam_str(char* param_name, char* value)
{
	ros_m_xmlrpc_call_setParam(param_name, value, NULL, 0);
}


void ros_m_nodeHandle_setParam_int32(char* param_name, int value)
{
	char int_str[13];

	sprintf(int_str, "i%d", value);
	ros_m_nodeHandle_setParam_str(param_name, int_str);
}


void ros_m_nodeHandle_setParam_float64(char* param_name, double value)
{
	char double_str[21];

	sprintf(double_str, "d%0.14lf", value);
	ros_m_nodeHandle_setParam_str(param_name, double_str);
}


void ros_m_nodeHandle_getParam_str(char* param_name, char* value)
{
	char res[64];
	char* rp = res;

	ros_m_xmlrpc_call_getParam(param_name, res, sizeof(res));

	while(*rp++ != '\n');
	while(*rp++ != '\n');

	while(*rp != 0)
		*value++ = *rp++;
}


void ros_m_nodeHandle_getParam_int32(char* param_name, int* value)
{
	char int_str[13];

	ros_m_nodeHandle_getParam_str(param_name, int_str);
	*value = atoi(int_str);
}


void ros_m_nodeHandle_getParam_float64(char* param_name, double* value)
{
	char double_str[21];

	ros_m_nodeHandle_getParam_str(param_name, double_str);
	*value = atof(double_str);
}


int ros_m_nodeHandle_advertise(MSG_FILE* message_file, char* topic_name, int queue_size, int latch)
{
	TOPIC* topic;

	topic = ros_m_topic_add_topic(message_file, topic_name, queue_size, latch, 1);
	if(topic == NULL)
	{
		return -1;
	}

	ros_m_xmlrpc_call_registerPublisher(topic);

	return 1;
}


int ros_m_nodeHandle_subscribe(MSG_FILE* message_file, char* topic_name, int queue_size, void (*callback)(void* param))
{
	char res[128];
	char* rp = res;
	char* pubp;
	char* pubtemp;
	char* publine;

	TOPIC* myTopic;
	int publisher_num = 0;

	myTopic = ros_m_topic_add_topic(message_file, topic_name, queue_size, 0, 0);
	if(myTopic == NULL)
	{
		return -1;
	}

	myTopic -> callback = callback;

	ros_m_xmlrpc_call_registerSubscriber(myTopic, res, sizeof(res));

	while(*rp++ != '\n');
	while(*rp++ != '\n');

	if(*rp != 'h' || *(rp + 1) != 't' || *(rp + 2) != 't' || *(rp + 3) != 'p')
	{
		return -1;
	}

	pubp = rp;
	while(*pubp != 0)
	{
		if(*pubp == '\n')
			publisher_num++;
		pubp++;
	}

	pubp = pubtemp = (char*)ros_m_pvPortMalloc(publisher_num * 30 + 1);
	memset(pubtemp, 0x00, publisher_num * 30 + 1);

	while(*rp != 0)
		*pubtemp++ = *rp++;
	*rp = 0;

	pubtemp = pubp;

	while(ros_m_ok())
	{
		publine = pubp;
		
		while(*pubp++ != ':');
		while(*pubp++ != ':');
		*(pubp - 1) = 0;

		if(ros_m_topic_find_connection(myTopic, inet_addr(publine + 7), atoi(pubp)) == NULL)
		{
			ros_m_nodeHandle_connect_topic(myTopic, publine + 7, atoi(pubp));
		}

		while(*pubp++ != '\n');
		if(*pubp == 0)
			break;
	}

	ros_m_vPortFree(pubtemp);

	return 1;
}


int ros_m_nodeHandle_publish(char* topic_name, void* msg_struct)
{
	TOPIC* topic;
	NODE* node;
	CONNECTION* connection;

	topic = ros_m_topic_get_topic(topic_name, strlen(topic_name), 1);
	if(topic == NULL)
	{
		return -1;
	}

	node = topic -> connection_linked_list -> head;

	while(node -> next != NULL)
	{
		connection = (CONNECTION*)(node -> next -> data);

		ros_m_message_enqueue_data(connection -> message_q, msg_struct);

		node = node -> next;
	}

	return 1;
}


int ros_m_nodeHandle_connect_topic(TOPIC* topic, char* ip, int port)
{
	char res[64];
	char* rp = res;

	ros_m_xmlrpc_call_requestTopic(topic, ip, port, res, sizeof(res));

	while(*rp++ != '\n');
	while(*rp++ != '\n');

	while(arg -> ip != 0);

	arg -> topic = topic;
	arg -> ip = inet_addr(rp);
	arg -> port[0] = port;

	while(*rp++ != '\n');
	arg -> port[1] = atoi(rp);

	if(ros_m_xTaskCreate(ROS_M_CONNECTION_TOPIC_RECV_TASK, "ROS_M_CONNECTION_TOPIC_RECV_TASK", TOPIC_RECV_CONNECTION_TASK_STACK_SIZE, (void*)arg, ROS_M_CONNECTION_TOPIC_RECV_TASK_PRIORITY, NULL) < 0)
		return -1;

	return 1;
}


void ros_m_nodeHandle_advertiseService(SERVICE_FILE* service_file, char* service_name, void (*callback)(void* param))
{
	SERVICE* service = ros_m_service_add_service(service_file, service_name, callback);

	ros_m_xmlrpc_call_registerService(service, NULL, 0);
}


int ros_m_nodeHandle_serviceClient(SERVICE_FILE* service_file, char* service_name, int persistance)
{
	char res[128];
	char* rp = res;
	char* ip;
	int port;

	SERVICE* service = ros_m_service_add_service(service_file, service_name, NULL);

	service -> persistance = 0;
	if(persistance == 1)
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

		service -> port = atoi(rp);
		*(rp - 1) = 0;

		service -> ip = inet_addr(ip);

		service -> persistance = 1;
	}

	return 1;
}


