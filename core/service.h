#ifndef SERVICE_H
#define SERVICE_H


#include "ros_m_opts.h"
#include "ros_m_portable.h"
#include "stdlib.h"


//==================================
#pragma pack(push)
#pragma pack(1)

#include "srv_file.h"

#pragma pack(pop)
//==================================


typedef struct SERVICE_FILE
{
	char* name;
	char* md5sum;
	char* definition;
} SERVICE_FILE;


typedef struct SERVICE_PROPERTIES
{
	char* name;
	int persistance;
	unsigned long ip;
	int port;
	void* data;
	int request_data_size;
	int response_data_size;
	void (*callback)(void* param);
	SERVICE_FILE* service_file;
} SERVICE;


#include "linked_list.h"
#include "connection.h"
#include "xmlrpc.h"
#include "tcpros.h"


void ros_m_service_init(void);

SERVICE_FILE* ros_m_service_create_service_file(char* name, char* md5sum, char* definition);

SERVICE* ros_m_service_add_service(SERVICE_FILE* service_file, char* service_name, void (*callback)(void* param));

SERVICE* ros_m_service_get_service(char* service_name, int name_len);

int ros_m_service_call(char* service_name, void* srv_struct);

void ros_m_service_delete_all_service(void);


#endif
