#ifndef CONNECTION_H
#define CONNECTION_H

#include "ros_m_opts.h"
#include "ros_m_portable.h"
#include "stdlib.h"
#include "topic.h"


#ifndef TCPROS_BUFFER_MAX_SIZE
#define TCPROS_BUFFER_MAX_SIZE 512
#endif

#ifndef TCPROS_INIT_BUFFER_MAX_SIZE
#define TCPROS_INIT_BUFFER_MAX_SIZE 1024
#endif


typedef struct TOPIC_RECV_TASK_PARAM{
	unsigned long ip;
	int port[2];
	TOPIC* topic;
} ARG;


#include "service.h"
#include "message.h"
#include "tcpros.h"
#include "init.h"


void ROS_M_CONNECTION_TOPIC_RECV_TASK(void* param);

void ROS_M_CONNECTION_TOPIC_SEND_SERVICE_SERVER_TASK(void* param);


#endif

