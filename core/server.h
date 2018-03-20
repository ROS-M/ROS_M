#ifndef SERVER_H
#define SERVER_H


#include "ros_m_opts.h"
#include "ros_m_portable.h"
#include "xmlrpc.h"
#include "connection.h"
#include "init.h"


#ifndef ROS_HOST_XMLRPC_PORT
#define ROS_HOST_XMLRPC_PORT 2000
#endif

#ifndef ROS_HOST_TCPROS_PORT
#define ROS_HOST_TCPROS_PORT 2001
#endif

#ifndef TOPIC_SEND_SERVICE_SERVER_CONNECTION_TASK_STACK_SIZE
#define TOPIC_SEND_SERVICE_SERVER_CONNECTION_TASK_STACK_SIZE 400
#endif


void ROS_M_SERVER_XMLRPC_TASK(void* param);

void ROS_M_SERVER_TCPROS_TASK(void* param);


#endif
