#ifndef ROS_M_PORTABLE_H
#define ROS_M_PORTABLE_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/stat.h>

#include <time.h>


typedef pthread_t ros_m_xTaskHandle;
typedef sem_t ros_m_xSemaphoreHandle;

typedef int ros_m_socket_t;
typedef struct sockaddr_in ros_m_socket_addr_t;

typedef long ros_m_tick_t;


int ros_m_xTaskCreate(void(*pxThread)(void* pvParameters), const char* threadName, int iStackSize, void* pvArg, int iPriority, ros_m_xTaskHandle* handle);


void ros_m_vTaskDelete(void* retrival);


int ros_m_xSemaphoreCreateCounting(ros_m_xSemaphoreHandle* semaphore, int count);


void ros_m_vSemaphoreDelete(ros_m_xSemaphoreHandle* semaphore);


int ros_m_xSemaphoreTake(ros_m_xSemaphoreHandle semaphore);


int ros_m_xSemaphoreGive(ros_m_xSemaphoreHandle semaphore);


ros_m_socket_t ros_m_lwip_socket(void);


int ros_m_setsockopt(int s, int level, int optname, const void* optval, uint32_t optlen);


void ros_m_TCP_set_socket_addr(ros_m_socket_addr_t* socket_addr, int socket_addr_len, unsigned long address, uint32_t port);


int ros_m_lwip_bind(ros_m_socket_t socket, ros_m_socket_addr_t* socket_addr, int socket_addr_len);


int ros_m_lwip_listen(ros_m_socket_t socket);


ros_m_socket_t ros_m_lwip_accept(ros_m_socket_t socket, ros_m_socket_addr_t* socket_addr, int socket_addr_len);


int ros_m_lwip_connect(ros_m_socket_t socket, ros_m_socket_addr_t* socket_addr, int socket_addr_len);


void ros_m_lwip_close(ros_m_socket_t socket);


int ros_m_lwip_recv(ros_m_socket_t socket, char* buffer, uint32_t len);


int ros_m_lwip_send(ros_m_socket_t socket, char* buffer, uint32_t len);


void* ros_m_pvPortMalloc(uint32_t size);


void ros_m_vPortFree(void* ptr);


ros_m_tick_t ros_m_get_tickCount(void);


void ros_m_delayUntil(ros_m_tick_t* tick, ros_m_tick_t freq);


void ros_m_delay(int delay);


#endif
