#include "tcpros.h"


const char* TCPROS_BUFFER1 = "callerid=/";
const char* TCPROS_BUFFER2 = "latching=0";
const char* TCPROS_BUFFER3 = "md5sum=";
const char* TCPROS_BUFFER4 = "message_definition=";
const char* TCPROS_BUFFER5 = "topic=/";
const char* TCPROS_BUFFER6 = "type=";
const char* TCPROS_BUFFER7 = "tcp_nodelay=0";

const char* TCPROS_BUFFER8 = "request_type=";
const char* TCPROS_BUFFER9 = "response_type=";
const char* TCPROS_BUFFER10 = "Request";
const char* TCPROS_BUFFER11 = "Response";
const char* TCPROS_BUFFER12 = "persistent=0";
const char* TCPROS_BUFFER13 = "service=/";


void ros_m_tcpros_parse_tcpros(char* buf)
{
	int total_len, line_len;
	int pointer_r = 4, pointer_w = 0, pointer_cp = 0;

	total_len = *(int*)buf;

	while(pointer_r < total_len + 4)
	{
		line_len = *(int*)(buf + pointer_r);
		pointer_r += 4;
		while(line_len--)
		{
			*(buf + pointer_w++) = *(buf + pointer_r++);
		}
		*(buf + pointer_w++) = '\n';
	}	
	*(buf + pointer_w) = 0;
}


int ros_m_tcpros_build_topic_tcpros(char* buf, TOPIC* topic)
{
	char* w = buf + 4;
	int pointer = 0;
	int len = 0;
	int total_len = 0;
	int isResponse = topic -> isPublisher;
	char* node_name = ros_m_get_node_name();

	len = strlen(TCPROS_BUFFER1) + strlen(node_name);
	total_len += len + 4;
	*(int*)w = len;
	w += 4;
	while(pointer < strlen(TCPROS_BUFFER1))
		*(char*)(w++) = *(TCPROS_BUFFER1 + pointer++);
	pointer = 0;
	while(pointer < strlen(node_name))
		*(char*)(w++) = *(node_name + pointer++);

	if(isResponse)
	{
		len = strlen(TCPROS_BUFFER2);
		total_len += len + 4;
		*(int*)w = len;
		w += 4;
		pointer = 0;
		while(pointer < len)
			*(char*)(w++) = *(TCPROS_BUFFER2 + pointer++);
	}

	len = strlen(TCPROS_BUFFER3) + strlen(topic -> message_file -> md5sum);
	total_len += len + 4;
	*(int*)w = len;
	w += 4;
	pointer = 0;
	while(pointer < strlen(TCPROS_BUFFER3))
		*(char*)(w++) = *(TCPROS_BUFFER3 + pointer++);
	pointer = 0;
	while(pointer < strlen(topic -> message_file -> md5sum))
		*(char*)(w++) = *(topic -> message_file -> md5sum + pointer++);

	if(isResponse)
	{
		len = strlen(TCPROS_BUFFER4) + strlen(topic -> message_file -> definition);
		total_len += len + 4;
		*(int*)w = len;
		w += 4;
		pointer = 0;
		while(pointer < strlen(TCPROS_BUFFER4))
			*(char*)(w++) = *(TCPROS_BUFFER4 + pointer++);
		pointer = 0;
		while(pointer < strlen(topic -> message_file -> definition))
			*(char*)(w++) = *(topic -> message_file -> definition + pointer++);
	}
	else
	{
		len = strlen(TCPROS_BUFFER7);
		total_len += len + 4;
		*(int*)w = len;
		w += 4;
		pointer = 0;
		while(pointer < len)
			*(char*)(w++) = *(TCPROS_BUFFER7 + pointer++);
	}

	len = strlen(TCPROS_BUFFER5) + strlen(topic -> name);
	total_len += len + 4;
	*(int*)w = len;
	w += 4;
	pointer = 0;
	while(pointer < strlen(TCPROS_BUFFER5))
		*(char*)(w++) = *(TCPROS_BUFFER5 + pointer++);
	pointer = 0;
	while(pointer < strlen(topic -> name))
		*(char*)(w++) = *(topic -> name + pointer++);
	
	len = strlen(TCPROS_BUFFER6) + strlen(PKG_NAME) + 1 + strlen(topic -> message_file -> name);
	total_len += len + 4;
	*(int*)w = len;
	w += 4;
	pointer = 0;
	while(pointer < strlen(TCPROS_BUFFER6))
		*(char*)(w++) = *(TCPROS_BUFFER6 + pointer++);
	pointer = 0;
	while(pointer < strlen(PKG_NAME))
		*(char*)(w++) = *(PKG_NAME + pointer++);
	pointer = 0;
	while(pointer++ < 1)
		*(char*)(w++) = '/';
	pointer = 0;
	while(pointer < strlen(topic -> message_file -> name))
		*(char*)(w++) = *(topic -> message_file -> name + pointer++);

	*(int*)buf = total_len;

	return total_len + 4;
}


int ros_m_tcpros_build_service_tcpros(char* buf, SERVICE* service, int isResponse)
{
	char* w = buf + 4;
	int pointer = 0;
	int len = 0;
	int total_len = 0;
	char* node_name = ros_m_get_node_name();

	len = strlen(TCPROS_BUFFER1) + strlen(node_name);
	total_len += len + 4;
	*(int*)w = len;
	w += 4;
	while(pointer < strlen(TCPROS_BUFFER1))
		*(char*)(w++) = *(TCPROS_BUFFER1 + pointer++);
	pointer = 0;
	while(pointer < strlen(node_name))
		*(char*)(w++) = *(node_name + pointer++);

	len = strlen(TCPROS_BUFFER3) + strlen(service -> service_file -> md5sum);
	total_len += len + 4;
	*(int*)w = len;
	w += 4;
	pointer = 0;
	while(pointer < strlen(TCPROS_BUFFER3))
		*(char*)(w++) = *(TCPROS_BUFFER3 + pointer++);
	pointer = 0;
	while(pointer < strlen(service -> service_file -> md5sum))
		*(char*)(w++) = *(service -> service_file -> md5sum + pointer++);

	if(isResponse)
	{
		len = strlen(TCPROS_BUFFER8) + strlen(PKG_NAME) + 1 + strlen(service -> service_file -> name) + strlen(TCPROS_BUFFER10);
		total_len += len + 4;
		*(int*)w = len;
		w += 4;
		pointer = 0;
		while(pointer < strlen(TCPROS_BUFFER8))
			*(char*)(w++) = *(TCPROS_BUFFER8 + pointer++);
		pointer = 0;
		while(pointer < strlen(PKG_NAME))
			*(char*)(w++) = *(PKG_NAME + pointer++);
		pointer = 0;
		while(pointer++ < 1)
			*(char*)(w++) = '/';
		pointer = 0;
		while(pointer < strlen(service -> service_file -> name))
			*(char*)(w++) = *(service -> service_file -> name + pointer++);
		pointer = 0;
		while(pointer < strlen(TCPROS_BUFFER10))
			*(char*)(w++) = *(TCPROS_BUFFER10 + pointer++);

		len = strlen(TCPROS_BUFFER9) + strlen(PKG_NAME) + 1 + strlen(service -> service_file -> name) + strlen(TCPROS_BUFFER11);
		total_len += len + 4;
		*(int*)w = len;
		w += 4;
		pointer = 0;
		while(pointer < strlen(TCPROS_BUFFER9))
			*(char*)(w++) = *(TCPROS_BUFFER9 + pointer++);
		pointer = 0;
		while(pointer < strlen(PKG_NAME))
			*(char*)(w++) = *(PKG_NAME + pointer++);
		pointer = 0;
		while(pointer++ < 1)
			*(char*)(w++) = '/';
		pointer = 0;
		while(pointer < strlen(service -> service_file -> name))
			*(char*)(w++) = *(service -> service_file -> name + pointer++);
		pointer = 0;
		while(pointer < strlen(TCPROS_BUFFER11))
			*(char*)(w++) = *(TCPROS_BUFFER11 + pointer++);
	}
	else
	{
		len = strlen(TCPROS_BUFFER12);
		total_len += len + 4;
		*(int*)w = len;
		w += 4;
		pointer = 0;
		while(pointer < len)
			*(char*)(w++) = *(TCPROS_BUFFER12 + pointer++);

		if(service -> persistance == 1)
			*(char*)(w - 1) = '1';
	}

	if(isResponse)
	{
		len = strlen(TCPROS_BUFFER6) + strlen(PKG_NAME) + 1 + strlen(service -> service_file -> name);
		total_len += len + 4;
		*(int*)w = len;
		w += 4;
		pointer = 0;
		while(pointer < strlen(TCPROS_BUFFER6))
			*(char*)(w++) = *(TCPROS_BUFFER6 + pointer++);
		pointer = 0;
		while(pointer < strlen(PKG_NAME))
			*(char*)(w++) = *(PKG_NAME + pointer++);
		pointer = 0;
		while(pointer++ < 1)
			*(char*)(w++) = '/';
		pointer = 0;
		while(pointer < strlen(service -> service_file -> name))
			*(char*)(w++) = *(service -> service_file -> name + pointer++);
	}
	else
	{
		len = strlen(TCPROS_BUFFER13) + strlen(service -> name);
		total_len += len + 4;
		*(int*)w = len;
		w += 4;
		pointer = 0;
		while(pointer < strlen(TCPROS_BUFFER13))
			*(char*)(w++) = *(TCPROS_BUFFER13 + pointer++);
		pointer = 0;
		while(pointer < strlen(service -> name))
			*(char*)(w++) = *(service -> name + pointer++);
	}

	*(int*)buf = total_len;

	return total_len + 4;
}


int ros_m_tcpros_build_message(char* buf, MESSAGE_Q* Q)
{
	int len = Q -> element_byte;
	*(int*)buf = len;
	
	return len + 4;
}


