#include "ros_m_portable.h"


int ros_m_xTaskCreate(void(*pxThread)(void* pvParameters), const char* threadName, int iStackSize, void* pvArg, int iPriority, ros_m_xTaskHandle* handle)
{
	ros_m_xTaskHandle res;
	return pthread_create(&res, NULL, (void*)(void*)pxThread, pvArg);
}


void ros_m_vTaskDelete(void* retrival)
{
	pthread_exit(retrival);
}


int ros_m_xSemaphoreCreateCounting(ros_m_xSemaphoreHandle* semaphore, int count)
{
	return sem_init(semaphore, 0, count);
}


void ros_m_vSemaphoreDelete(ros_m_xSemaphoreHandle* semaphore)
{
	//sem_delete(semaphore);
	//*semaphore == NULL;
}


int ros_m_xSemaphoreTake(ros_m_xSemaphoreHandle semaphore)
{
	return sem_wait(&semaphore);
}


int ros_m_xSemaphoreGive(ros_m_xSemaphoreHandle semaphore)
{
	return sem_post(&semaphore);
}


ros_m_socket_t ros_m_lwip_socket()
{
	return socket(AF_INET, SOCK_STREAM, 0);
}


int ros_m_setsockopt(int s, int level, int optname, const void* optval, uint32_t optlen)
{
	return setsockopt(s, level, optname, optval, optlen);
}


void ros_m_TCP_set_socket_addr(ros_m_socket_addr_t* socket_addr, int socket_addr_len, unsigned long address, uint32_t port)
{
	memset(socket_addr, 0x00, socket_addr_len);
	socket_addr->sin_family = AF_INET;
	socket_addr->sin_port = htons(port);
	socket_addr->sin_addr.s_addr = address;
}


int ros_m_lwip_bind(ros_m_socket_t socket, ros_m_socket_addr_t* socket_addr, int socket_addr_len)
{
	return bind(socket, (struct sockaddr*)socket_addr, socket_addr_len);
}


int ros_m_lwip_listen(ros_m_socket_t socket)
{
	return listen(socket, 10);
}


ros_m_socket_t ros_m_lwip_accept(ros_m_socket_t socket, ros_m_socket_addr_t* socket_addr, int socket_addr_len)
{
	int len = sizeof(socket_addr);

	return accept(socket, (struct sockaddr*)socket_addr, &len);
}


int ros_m_lwip_connect(ros_m_socket_t socket, ros_m_socket_addr_t* socket_addr, int socket_addr_len)
{
	return connect(socket, (struct sockaddr*)socket_addr, socket_addr_len);
}


void ros_m_lwip_close(ros_m_socket_t socket)
{
	close(socket);
}


int ros_m_lwip_recv(ros_m_socket_t socket, char* buffer, uint32_t len)
{
	return read(socket, buffer, len, 0);
}


int ros_m_lwip_send(ros_m_socket_t socket, char* buffer, uint32_t len)
{
	return write(socket, buffer, len, 0);
}


void* ros_m_pvPortMalloc(uint32_t size)
{
	return malloc(size);
}


void ros_m_vPortFree(void* ptr)
{
	free(ptr);
}


ros_m_tick_t ros_m_get_tickCount()
{
	struct timespec tick;
	gettimeofday (&tick, NULL);
	
	return (tick.tv_sec*1000 + tick.tv_nsec/1000);
}


void ros_m_delayUntil(ros_m_tick_t* tick, ros_m_tick_t freq)
{
	sleep(1);
}


void ros_m_delay(int delay)
{
	sleep(delay);
}




