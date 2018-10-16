#ifndef XMLRPC_H
#define XMLRPC_H


#include "ros_m_opts.h"
#include "ros_m_portable.h"
#include "stdlib.h"
#include "topic.h"
#include "service.h"
#include "http.h"
#include "init.h"
#include "node_handle.h"


#ifndef XMLRPC_BUFFER_MAX_SIZE
#define XMLRPC_BUFFER_MAX_SIZE 1024
#endif

#ifndef ROS_MASTER_PORT
#define ROS_MASTER_PORT 11311
#endif

#ifndef ROS_MASTER_IP
#define ROS_MASTER_IP "10.0.0.1"
#endif

#ifndef ROS_HOST_IP
#define ROS_HOST_IP "10.0.0.2"
#endif


#define RPC_HASPARAM 1
#define RPC_SETPARAM 2
#define RPC_GETPARAM 3
#define RPC_REGISTERPUBLISHER 4
#define RPC_REGISTERSUBSCRIBER 5
#define RPC_REQUESTTOPIC_REQ 6
#define RPC_REQUESTTOPIC_RES 7
#define RPC_PUBLISHERUPDATE_RES 8
#define RPC_REGISTERSERVICE 9
#define RPC_LOOKUPSERVICE 10
#define RPC_UNREGISTERPUBLISHER 11
#define RPC_UNREGISTERSUBSCRIBER 12
#define RPC_UNREGISTERSERVICE 13


int ros_m_xmlrpc_request_response(char* ip, int port, char* xml_buf, int xml_buf_len);

int ros_m_xmlrpc_build_request_p2(char* buf, int api_index, char* param1, char* param2);

int ros_m_xmlrpc_build_request_p1(char* buf, int api_index, char* param1);

int ros_m_xmlrpc_build_request_p0(char* buf, int api_index);

int ros_m_xmlrpc_parse_xmlrpc(char* wbuf, int wbuf_len, char* rbuf, int rbuf_len);

int ros_m_xmlrpc_call_hasParam(char* param_name, char* return_ptr, int return_ptr_len);

void ros_m_xmlrpc_call_setParam(char* param_name, char* param_value, char* return_ptr, int return_ptr_len);

int ros_m_xmlrpc_call_getParam(char* param_name, char* return_ptr, int return_ptr_len);

void ros_m_xmlrpc_call_registerPublisher(TOPIC* topic);

int ros_m_xmlrpc_call_registerSubscriber(TOPIC* topic, char* return_ptr, int return_ptr_len);

int ros_m_xmlrpc_call_requestTopic(TOPIC* topic, char* ip, int port, char* return_ptr, int return_ptr_len);

void ros_m_xmlrpc_call_registerService(SERVICE* service, char* return_ptr, int return_ptr_len);

int ros_m_xmlrpc_call_lookupService(char* service_name, char* return_ptr, int return_ptr_len);

void ros_m_xmlrpc_call_unregisterPublisher(TOPIC* topic, char* return_ptr, int return_ptr_len);

void ros_m_xmlrpc_call_unregisterSubscriber(TOPIC* topic, char* return_ptr, int return_ptr_len);

void ros_m_xmlrpc_call_unregisterService(SERVICE* service, char* return_ptr, int return_ptr_len);

int ros_m_xmlrpc_api_requestTopic(char* buf, int buf_len, int client_sockfd);

void ros_m_xmlrpc_api_publisherUpdate(char* buf, int buf_len, int client_sockfd);

void ros_m_xmlrpc_api_shutdown(void);


#endif
