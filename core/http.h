#ifndef HTTP_H
#define HTTP_H


#include "ros_m_opts.h"
#include "ros_m_portable.h"
#include "stdlib.h"


int ros_m_http_build_header(char* ip, int port, char* buf, int buf_len, int isRequest);

int ros_m_http_parse_header(char* buf, int buf_len);


#endif
