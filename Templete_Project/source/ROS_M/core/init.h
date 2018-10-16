#ifndef INIT_H
#define INIT_H


#include "ros_m_opts.h"
#include "ros_m_portable.h"
#include "topic.h"
#include "service.h"
#include "message.h"
#include <string.h>


#ifndef NODE_NAME_MAX_LENGTH
#define NODE_NAME_MAX_LENGTH 32
#endif

#ifndef CALLBACK_PARAM_MAX_SIZE
#define CALLBACK_PARAM_MAX_SIZE 256
#endif


int ros_m_init(int argc, char** argv, char* node_name);

char* ros_m_get_node_name(void);

int ros_m_ok(void);

void ros_m_shutdown(void);

void ros_m_spin(void);

void ros_m_spinOnce(void);

void ros_m_call_callbacks(void* callback_Param);


#endif
