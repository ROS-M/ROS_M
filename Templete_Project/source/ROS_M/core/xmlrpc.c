#include "xmlrpc.h"


const char* XMLRPC_BUFFER1 = "\r\n\r\n<?xml version=\"1.0\"?><methodCall><methodName>";
const char* XMLRPC_BUFFER2 = "</methodName><params><param><value>";
const char* XMLRPC_BUFFER3 = "</value></param><param><value>";
const char* XMLRPC_BUFFER4 = "</value></param></params></methodCall>";

const char* XMLRPC_BUFFER5 = "\r\n\r\n<?xml version=\"1.0\"?><methodResponse><params><param>\t<value><array><data><value><i4>1</i4></value><value></value><value>";
const char* XMLRPC_BUFFER6 = "</value><value><i4>";
const char* XMLRPC_BUFFER7 = "</i4></value></data></array></value></data></array></value></param></params></methodResponse>";

const char* XMLRPC_BUFFER8 = "<array><data><value><array><data><value>TCPROS</value></data></array></value></data></array>";
const char* XMLRPC_BUFFER9 = "<array><data><value>TCPROS</value><value>";
const char* XMLRPC_BUFFER10 = "<i4>0</i4></value></data></array></value></param></params></methodResponse>";


char xmlrpc_Buffer_call[XMLRPC_BUFFER_MAX_SIZE];
extern ros_m_xSemaphoreHandle xmlrpc_Buffer_call_sem;


int ros_m_xmlrpc_request_response(char* ip, int port, char* xml_buf, int xml_buf_len)
{
	struct sockaddr_in serveraddr;
	int server_sockfd;
	int connected = 0;

	while(connected == 0)
	{
		if((server_sockfd = ros_m_lwip_socket()) == -1)
        	{
			return -1;
		}

		ros_m_TCP_set_socket_addr(&serveraddr, sizeof(serveraddr), inet_addr(ip), port);

		if(ros_m_lwip_connect(server_sockfd, &serveraddr, sizeof(serveraddr)) == -1)
        	{
			ros_m_lwip_close(server_sockfd);
			ros_m_delay(50);
		}
		else 
			connected = 1;
	}

	if(ros_m_lwip_send(server_sockfd, xml_buf, xml_buf_len) <= 0)
        {
		ros_m_lwip_close(server_sockfd);

		return -1;
        }

	if(ros_m_lwip_send(server_sockfd, xml_buf, xml_buf_len) <= 0)
        {
		ros_m_lwip_close(server_sockfd);

		return -1;
        }

	memset(xml_buf, 0x00, xml_buf_len);
	ros_m_lwip_recv(server_sockfd, xml_buf, XMLRPC_BUFFER_MAX_SIZE);

	ros_m_lwip_close(server_sockfd);

	return 1;
}


int ros_m_xmlrpc_build_request_p2(char* buf, int api_index, char* param1, char* param2)
{
	char body_length_str[5] = "\r\r\r\r\r";
	int body_length;
	int header_length = strlen(buf);
	int pointer = 0;
	char* node_name = ros_m_get_node_name();

	switch(api_index)
	{
	case RPC_HASPARAM:
		sprintf(buf + header_length, "%s%s%s/%s%s/%s%s", XMLRPC_BUFFER1, "hasParam", XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER4);
		break;
	
	case RPC_SETPARAM:
		if(param2[0] == 'i')
			sprintf(buf + header_length, "%s%s%s<string>%s</string>%s<string>%s</string>%s<int>%s</int>%s", 
				XMLRPC_BUFFER1, "setParam", XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, param2 + 1, XMLRPC_BUFFER4);
		else if(param2[0] == 'd')
			sprintf(buf + header_length, "%s%s%s<string>%s</string>%s<string>%s</string>%s<double>%s</double>%s", 
				XMLRPC_BUFFER1, "setParam", XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, param2 + 1, XMLRPC_BUFFER4);
		else
			sprintf(buf + header_length, "%s%s%s<string>%s</string>%s<string>%s</string>%s<string>%s</string>%s", 
				XMLRPC_BUFFER1, "setParam", XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, param2, XMLRPC_BUFFER4);
		break;

	case RPC_GETPARAM:
		sprintf(buf + header_length, "%s%s%s<string>/%s</string>%s<string>/%s</string>%s", XMLRPC_BUFFER1, "getParam", XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER4);

		break;
	case RPC_REGISTERPUBLISHER:
		sprintf(buf + header_length, "%s%s%s/%s%s/%s%s%s/%s%shttp://%s:%d/%s", XMLRPC_BUFFER1, "registerPublisher", 
			XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, PKG_NAME, param2, XMLRPC_BUFFER3, ROS_HOST_IP, ROS_HOST_XMLRPC_PORT, XMLRPC_BUFFER4);
		break;

	case RPC_REGISTERSUBSCRIBER:
		sprintf(buf + header_length, "%s%s%s/%s%s/%s%s%s/%s%shttp://%s:%d/%s", XMLRPC_BUFFER1, "registerSubscriber", 
			XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, PKG_NAME, param2, XMLRPC_BUFFER3, ROS_HOST_IP, ROS_HOST_XMLRPC_PORT, XMLRPC_BUFFER4);
		break;

	case RPC_REQUESTTOPIC_REQ:
		sprintf(buf + header_length, "%s%s%s/%s%s/%s%s%s%s", XMLRPC_BUFFER1, "requestTopic", XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, XMLRPC_BUFFER8, XMLRPC_BUFFER4);
		break;

	case RPC_REQUESTTOPIC_RES:
		sprintf(buf + header_length, "%s%s%s%s%d%s", XMLRPC_BUFFER5, XMLRPC_BUFFER9, ROS_HOST_IP, XMLRPC_BUFFER6, ROS_HOST_TCPROS_PORT, XMLRPC_BUFFER7);
		break;

	case RPC_PUBLISHERUPDATE_RES:
		sprintf(buf + header_length, "%s%s", XMLRPC_BUFFER5, XMLRPC_BUFFER10);
		break;
	case RPC_REGISTERSERVICE:
		sprintf(buf + header_length, "%s%s%s/%s%s/%s%srosrpc://%s:%d/%shttp://%s:%d/%s", XMLRPC_BUFFER1, "registerService", 
			XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, ROS_HOST_IP, ROS_HOST_TCPROS_PORT, XMLRPC_BUFFER3, ROS_HOST_IP, ROS_HOST_XMLRPC_PORT, XMLRPC_BUFFER4);
		break;

	case RPC_LOOKUPSERVICE:
		sprintf(buf + header_length, "%s%s%s/%s%s/%s%s", XMLRPC_BUFFER1, "lookupService", XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER4);
		break;

	case RPC_UNREGISTERPUBLISHER:
		sprintf(buf + header_length, "%s%s%s/%s%s/%s%shttp://%s:%d/%s", XMLRPC_BUFFER1, "unregisterPublisher", 
			XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, ROS_HOST_IP, ROS_HOST_XMLRPC_PORT, XMLRPC_BUFFER4);
		break;
	case RPC_UNREGISTERSUBSCRIBER:
		sprintf(buf + header_length, "%s%s%s/%s%s/%s%shttp://%s:%d/%s", XMLRPC_BUFFER1, "unregisterSubscriber", 
			XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, ROS_HOST_IP, ROS_HOST_XMLRPC_PORT, XMLRPC_BUFFER4);
		break;
	case RPC_UNREGISTERSERVICE:
		sprintf(buf + header_length, "%s%s%s/%s%s/%s%srosrpc://%s:%d/%s", XMLRPC_BUFFER1, "unregisterService", 
			XMLRPC_BUFFER2, node_name, XMLRPC_BUFFER3, param1, XMLRPC_BUFFER3, ROS_HOST_IP, ROS_HOST_TCPROS_PORT, XMLRPC_BUFFER4);
		break;
	}

	body_length = strlen(buf) - header_length - 4;
	sprintf(body_length_str, "%d", body_length);

	while(body_length_str[pointer] != 0)
	{
		*(buf + header_length - 5 + pointer) = *(body_length_str + pointer);
		pointer++;
	}

	return strlen(buf + header_length);
}


int ros_m_xmlrpc_build_request_p1(char* buf, int api_index, char* param1)
{
	return ros_m_xmlrpc_build_request_p2(buf, api_index, param1, "");
}


int ros_m_xmlrpc_build_request_p0(char* buf, int api_index)
{
	return ros_m_xmlrpc_build_request_p2(buf, api_index, "", "");
}


int ros_m_xmlrpc_parse_xmlrpc(char* wbuf, int wbuf_len, char* rbuf, int rbuf_len)
{
	char* rp =rbuf;
	char* wp = wbuf;
	int xml_type = 0;

	while(rp - rbuf < rbuf_len)
	{
		while(*rp++ != '<' && rp - rbuf < rbuf_len);
		if(!xml_type && *(rp + 6) == 'C') xml_type = 1;/* methodCall 의미 */
		else if(!xml_type && *(rp + 6) == 'R') xml_type = 2;/* methodResponse 의미 */

		if(xml_type == 1)
		{
			if(*rp == 'm' || *rp == 's' || *rp == 'v' ||  *(rp + 1) == 'm')
			{
				if(strncmp(rp, "methodName>", 11) == 0)
				{
					while(*(rp + 11) != '<')
					{
						*(wp++) = *(rp++ + 11);
						if(wp - wbuf >= wbuf_len) return -1;/* wbuf 크기 부족 */
					}
					if(*(rp + 10) != '>') *(wp++) = '\n';
					if(wp - wbuf >= wbuf_len) return -1;
				}
				else if(strncmp(rp, "string>", 7) == 0)
				{
					while(*(rp + 7) != '<')
					{
						*(wp++) = *(rp++ + 7);
						if(wp - wbuf >= wbuf_len) return -1;
					}
					if(*(rp + 6) != '>') *(wp++) = '\n';
					if(wp - wbuf >= wbuf_len) return -1;
				}
				else if(strncmp(rp, "value>", 6) == 0)
				{
					while(*(rp + 6) != '<')
					{
						*(wp++) = *(rp++ + 6);
						if(wp - wbuf >= wbuf_len) return -1;
					}
					if(*(rp + 5) != '>') *(wp++) = '\n';
					if(wp - wbuf >= wbuf_len) return -1;
				}
				else if(strncmp(rp, "/methodCall>", 12) == 0)
				{
					*wp = 0;
					break;
				}
			}
		}
		else if(xml_type == 2)
		{
			if(*rp == 'i' || *rp == 'v' || *rp == 'd' || *rp == 's' || *rp == 'b' || *(rp + 1) == 'm')
			{
				if(strncmp(rp, "int>", 4) == 0)
				{
					while(*(rp + 4) != '<')
					{
						*(wp++) = *(rp++ + 4);
						if(wp - wbuf >= wbuf_len) return -1;
					}
					if(*(rp + 3) != '>') *(wp++) = '\n';
					if(wp - wbuf >= wbuf_len) return -1;
				}
				else if(strncmp(rp, "value>", 6) == 0)
				{
					while(*(rp + 6) != '<')
					{
						*(wp++) = *(rp++ + 6);
						if(wp - wbuf >= wbuf_len) return -1;
					}
					if(*(rp + 5) != '>') *(wp++) = '\n';
					if(wp - wbuf >= wbuf_len) return -1;
				}
				else if(strncmp(rp, "i4>", 3) == 0)
				{
					while(*(rp + 3) != '<')
					{
						*(wp++) = *(rp++ + 3);
						if(wp - wbuf >= wbuf_len) return -1;
					}
					if(*(rp + 2) != '>') *(wp++) = '\n';
					if(wp - wbuf >= wbuf_len) return -1;
				}
				else if(strncmp(rp, "double>", 7) == 0)
				{
					while(*(rp + 7) != '<')
					{
						*(wp++) = *(rp++ + 7);
						if(wp - wbuf >= wbuf_len) return -1;
					}
					if(*(rp + 6) != '>') *(wp++) = '\n';
					if(wp - wbuf >= wbuf_len) return -1;
				}
				else if(strncmp(rp, "string>", 7) == 0)
				{
					while(*(rp + 7) != '<')
					{
						*(wp++) = *(rp++ + 7);
						if(wp - wbuf >= wbuf_len) return -1;
					}
					if(*(rp + 6) != '>') *(wp++) = '\n';
					if(wp - wbuf >= wbuf_len) return -1;
				}
				else if(strncmp(rp, "boolean>", 8) == 0)
				{
					while(*(rp + 8) != '<')
					{
						*(wp++) = *(rp++ + 8);
						if(wp - wbuf >= wbuf_len) return -1;
					}
					if(*(rp + 7) != '>') *(wp++) = '\n';
					if(wp - wbuf >= wbuf_len) return -1;
				}
				else if(strncmp(rp, "/methodResponse>", 16) == 0)
				{
					*wp = 0;
					break;
				}				
			}
		}
		
	}
	return 1;
}

int ros_m_xmlrpc_call_hasParam(char* param_name, char* return_ptr, int return_ptr_len)
{
	int len;
	int ret = 1;
	
	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p1(xmlrpc_Buffer_call, RPC_HASPARAM, param_name);

	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, len);

	if(ros_m_http_parse_header(xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) == -1)
		ret = -1;

	if(ros_m_xmlrpc_parse_xmlrpc(return_ptr, return_ptr_len, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) < 0)
		ret = -1;

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);

	return ret;
}


void ros_m_xmlrpc_call_setParam(char* param_name, char* param_value, char* return_ptr, int return_ptr_len)
{
	int len;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p2(xmlrpc_Buffer_call, RPC_SETPARAM, param_name, param_value);
	
	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, len);

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);
}


int ros_m_xmlrpc_call_getParam(char* param_name, char* return_ptr, int return_ptr_len)
{
	int len;
	int ret = 1;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p1(xmlrpc_Buffer_call, RPC_GETPARAM, param_name);

	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, len);
	
	if(ros_m_http_parse_header(xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) == -1)
		ret = -1;

	if(ros_m_xmlrpc_parse_xmlrpc(return_ptr, return_ptr_len, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) < 0)
		ret = -1;

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);

	return ret;
}


void ros_m_xmlrpc_call_registerPublisher(TOPIC* topic)
{
	int len;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p2(xmlrpc_Buffer_call, RPC_REGISTERPUBLISHER, topic -> name, topic -> message_file -> name);

	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, len);

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);
}


int ros_m_xmlrpc_call_registerSubscriber(TOPIC* topic, char* return_ptr, int return_ptr_len)
{
	int len;
	int ret = 1;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p2(xmlrpc_Buffer_call, RPC_REGISTERSUBSCRIBER, topic -> name, topic -> message_file -> name);

	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, len);
	
	if(ros_m_http_parse_header(xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) == -1)
		ret = -1;

	if(ros_m_xmlrpc_parse_xmlrpc(return_ptr, return_ptr_len, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) < 0)
		ret = -1;

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);

	return ret;
}


int ros_m_xmlrpc_call_requestTopic(TOPIC* topic, char* ip, int port, char* return_ptr, int return_ptr_len)
{
	int len;
	int ret = 1;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ip, port, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p1(xmlrpc_Buffer_call, RPC_REQUESTTOPIC_REQ, topic -> name);

	ros_m_xmlrpc_request_response(ip, port, xmlrpc_Buffer_call, len);

	if(ros_m_http_parse_header(xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) == -1)
		ret = -1;

	if(ros_m_xmlrpc_parse_xmlrpc(return_ptr, return_ptr_len, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) < 0)
		ret = -1;

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);

	return ret;
}


void ros_m_xmlrpc_call_registerService(SERVICE* service, char* return_ptr, int return_ptr_len)
{
	int len;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p2(xmlrpc_Buffer_call, RPC_REGISTERSERVICE, service -> name, service -> service_file -> name);

	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, len);

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);
}


int ros_m_xmlrpc_call_lookupService(char* service_name, char* return_ptr, int return_ptr_len)
{
	int len;
	int ret = 1;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p1(xmlrpc_Buffer_call, RPC_LOOKUPSERVICE, service_name);

	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, len);

	if(ros_m_http_parse_header(xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) == -1)
		ret = -1;

	if(ros_m_xmlrpc_parse_xmlrpc(return_ptr, return_ptr_len, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE) < 0)
		ret = -1;

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);

	return ret;
}


void ros_m_xmlrpc_call_unregisterPublisher(TOPIC* topic, char* return_ptr, int return_ptr_len)
{
	int len;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p1(xmlrpc_Buffer_call, RPC_UNREGISTERPUBLISHER, topic -> name);

	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, len);

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);
}


void ros_m_xmlrpc_call_unregisterSubscriber(TOPIC* topic, char* return_ptr, int return_ptr_len)
{
	int len;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p1(xmlrpc_Buffer_call, RPC_UNREGISTERSUBSCRIBER, topic -> name);

	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, len);

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);
}


void ros_m_xmlrpc_call_unregisterService(SERVICE* service, char* return_ptr, int return_ptr_len)
{
	int len;

	ros_m_xSemaphoreTake(xmlrpc_Buffer_call_sem);
	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE, 1);
	len += ros_m_xmlrpc_build_request_p1(xmlrpc_Buffer_call, RPC_UNREGISTERSERVICE, service -> name);

	ros_m_xmlrpc_request_response(ROS_MASTER_IP, ROS_MASTER_PORT, xmlrpc_Buffer_call, XMLRPC_BUFFER_MAX_SIZE);

	ros_m_xSemaphoreGive(xmlrpc_Buffer_call_sem);
}


int ros_m_xmlrpc_api_requestTopic(char* buf, int buf_len, int client_sockfd)
{
	int len;

	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, buf, buf_len, 0);
	len += ros_m_xmlrpc_build_request_p0(buf, RPC_REQUESTTOPIC_RES);

	if(ros_m_lwip_send(client_sockfd, buf, len) <= 0)
		return -1;

	return 1;
}


void ros_m_xmlrpc_api_publisherUpdate(char* buf, int buf_len, int client_sockfd)
{
	char* rp;
	char* pubp;
	char* pubtemp;
	char* publine;

	TOPIC* myTopic;
	int topic_name_len = 0;
	int publisher_num = 0;
	int len;

	rp = buf;
	while(*rp++ != '\n');
	while(*rp++ != '\n');	
	while(*rp++ != '\n')
		topic_name_len++;

	myTopic = ros_m_topic_get_topic(rp - topic_name_len, topic_name_len - 1, 0);
	if(myTopic == NULL)
	{
		return;
	}

	if(*rp != 'h' || *(rp + 1) != 't' || *(rp + 2) != 't' || *(rp + 3) != 'p')
	{
		return;
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

	len = ros_m_http_build_header(ROS_MASTER_IP, ROS_MASTER_PORT, buf, buf_len, 0);
	len += ros_m_xmlrpc_build_request_p0(buf, RPC_PUBLISHERUPDATE_RES);

	if(ros_m_lwip_send(client_sockfd, buf, len) <= 0)
		;			

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
}


void ros_m_xmlrpc_api_shutdown()
{
	;
}


