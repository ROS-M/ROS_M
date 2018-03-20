#include "http.h"


const char* HTTP_BUFFER1 = "POST / HTTP/1.1\r\nUser-Agent: XMLRPC++ 0.7\r\nHost: ";
const char* HTTP_BUFFER2 = "\r\nContent-Type: text/xml\r\nContent-length: ";
const char* HTTP_BUFFER3 = "HTTP/1.0 200 OK\r\nServer: BaseHTTP/0.3 Python/2.7.6\r\nDate: Fri, 02 Oct 2015 09:39:28 GMT\r\nContent-type: text/xml\r\nContent-length: ";



int ros_m_http_build_header(char* ip, int port, char* buf, int buf_len, int isRequest)
{
	memset(buf, 0x00, buf_len);

	if(isRequest) sprintf(buf, "%s%s:%d%s     ", HTTP_BUFFER1, ip, port, HTTP_BUFFER2);
	else sprintf(buf, "%s     ", HTTP_BUFFER3);
	
	return strlen(buf);
}


int ros_m_http_parse_header(char* buf, int buf_len)
{
	char* p = buf;
	if(strncmp(buf, "HTTP/1.0 200 OK", 15) == 0 || strncmp(buf, "HTTP/1.1 200 OK", 15) == 0 || strncmp(buf, "POST / HTTP/1.1", 15) == 0)
	{
		while(p - buf < buf_len)
		{
			while(*p++ != '\n' && p - buf < buf_len);
			if(*p == 'C')
				if(strncmp(p, "Content-length: ", 16) == 0 || strncmp(p, "Content-Length: ", 16) == 0)
					return atoi(p + 16);
		}
	}

	return -1;
}



