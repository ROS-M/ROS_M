#ifndef NODE_HANDLE_H
#define NODE_HANDLE_H


#include "ros_m_opts.h"
#include "ros_m_portable.h"
#include "stdlib.h"
#include "topic.h"
#include "service.h"
#include "server.h"
#include "xmlrpc.h"
#include "message.h"
#include "init.h"


#ifndef XMLRPC_SERVER_TASK_STACK_SIZE
#define XMLRPC_SERVER_TASK_STACK_SIZE 400
#endif

#ifndef TCPROS_SERVER_TASK_STACK_SIZE
#define TCPROS_SERVER_TASK_STACK_SIZE 400
#endif

#ifndef TOPIC_RECV_CONNECTION_TASK_STACK_SIZE
#define TOPIC_RECV_CONNECTION_TASK_STACK_SIZE 400
#endif


int ros_m_nodeHandle_init(void);

int ros_m_nodeHandle_hasParam_str(char* param_name);

void ros_m_nodeHandle_setParam_str(char* param_name, char* value);

void ros_m_nodeHandle_setParam_int32(char* param_name, int value);

void ros_m_nodeHandle_setParam_float64(char* param_name, double value);

void ros_m_nodeHandle_getParam_str(char* param_name, char* value);

void ros_m_nodeHandle_getParam_int32(char* param_name, int* value);

void ros_m_nodeHandle_getParam_float64(char* param_name, double* value);

int ros_m_nodeHandle_advertise(MSG_FILE* message, char* topic_name, int queue_size, int latch);

int ros_m_nodeHandle_subscribe(MSG_FILE* message, char* topic_name, int queue_size, void (*callback)(void* param));

int ros_m_nodeHandle_publish(char* topic_name, void* msg_struct);

int ros_m_nodeHandle_connect_topic(TOPIC* topic, char* ip, int port);

void ros_m_nodeHandle_advertiseService(SERVICE_FILE* service_file, char* service_name, void (*callback)(void* param));

int ros_m_nodeHandle_serviceClient(SERVICE_FILE* service_file, char* service_name, int persistance);


#endif
