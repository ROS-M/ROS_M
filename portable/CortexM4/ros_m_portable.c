#include "ros_m_portable.h"


int ros_m_xTaskCreate(void(*pxThread)(void* pvParameters), const char* threadName, int iStackSize, void* pvArg, int iPriority, ros_m_xTaskHandle* handle)
{
	return xTaskCreate(pxThread, threadName, iStackSize, pvArg, iPriority, handle);
}


void ros_m_vTaskDelete(void* retrival)
{
	vTaskDelete(retrival);
}


int ros_m_xSemaphoreCreateCounting(ros_m_xSemaphoreHandle* semaphore, int count)
{
	*semaphore = xSemaphoreCreateCounting(count, count);
	if (*semaphore == NULL) {
		return -1;
	}
	return 0;
}


void ros_m_vSemaphoreDelete(ros_m_xSemaphoreHandle* semaphore)
{
	vSemaphoreDelete(semaphore);
	*semaphore == NULL;
}


int ros_m_xSemaphoreTake(ros_m_xSemaphoreHandle semaphore)
{
	 return xSemaphoreTake(semaphore, portMAX_DELAY);
}


int ros_m_xSemaphoreGive(ros_m_xSemaphoreHandle semaphore)
{
	return xSemaphoreGive(semaphore);
}


ros_m_socket_t ros_m_lwip_socket()
{
	return lwip_socket(AF_INET, SOCK_STREAM, 0);
}


int ros_m_setsockopt(int s, int level, int optname, const void* optval, uint32_t optlen)
{
	return setsockopt(s, level, optname, optval, optlen);
}


void ros_m_TCP_set_socket_addr(ros_m_socket_addr_t* socket_addr, int socket_addr_len, unsigned long address, uint32_t port)
{
	memset(socket_addr, 0x00, socket_addr_len);
	socket_addr->sin_len = socket_addr_len;
	socket_addr->sin_family = AF_INET;
	socket_addr->sin_port = PP_HTONS(port);
	socket_addr->sin_addr.s_addr = address;
}


int ros_m_lwip_bind(ros_m_socket_t socket, ros_m_socket_addr_t* socket_addr, int socket_addr_len)
{
	return lwip_bind(socket, (struct sockaddr*)socket_addr, socket_addr_len);
}


int ros_m_lwip_listen(ros_m_socket_t socket)
{
	return lwip_listen(socket, 10);
}


ros_m_socket_t ros_m_lwip_accept(ros_m_socket_t socket, ros_m_socket_addr_t* socket_addr, int socket_addr_len)
{
	int len = sizeof(socket_addr);

	return lwip_accept(socket, (struct sockaddr*)socket_addr, &len);
}


int ros_m_lwip_connect(ros_m_socket_t socket, ros_m_socket_addr_t* socket_addr, int socket_addr_len)
{
	return lwip_connect(socket, (struct sockaddr*)socket_addr, socket_addr_len);
}


void ros_m_lwip_close(ros_m_socket_t socket)
{
	lwip_close(socket);
}


int ros_m_lwip_recv(ros_m_socket_t socket, char* buffer, uint32_t len)
{
	return lwip_recv(socket, buffer, len, 0);
}


int ros_m_lwip_send(ros_m_socket_t socket, char* buffer, uint32_t len)
{
	return lwip_send(socket, buffer, len, 0);
}


void* ros_m_pvPortMalloc(uint32_t size)
{
	return pvPortMalloc(size);
}


void ros_m_vPortFree(void* ptr)
{
	vPortFree(ptr);
}


ros_m_tick_t ros_m_get_tickCount()
{
	return xTaskGetTickCount();
}


void ros_m_delayUntil(ros_m_tick_t* tick, ros_m_tick_t freq)
{
	ros_m_delayUntil(tick, freq);
}


void ros_m_delay(int delay)
{
	vTaskDelay(delay);
}



