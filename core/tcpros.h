#ifndef TCPROS_H
#define TCPROS_H


#include "ros_m_opts.h"
#include "topic.h"
#include "service.h"


void ros_m_tcpros_parse_tcpros(char* buf);

int ros_m_tcpros_build_topic_tcpros(char* buf, TOPIC* topic);

int ros_m_tcpros_build_service_tcpros(char* buf, SERVICE* service, int isResponse);

int ros_m_tcpros_build_message(char* buf, MESSAGE_Q* Q);


#endif

