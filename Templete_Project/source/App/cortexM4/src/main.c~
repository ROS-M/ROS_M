#include "includes.h"


static void Task_Init(void *p_arg);
static void Task_ROS_M_Init(void *p_arg);
static void Task_ROS_M_Spin(void *p_arg);


int main()
{
	xTaskCreate(Task_Init, "INIT", 500, NULL, tskIDLE_PRIORITY, NULL);

	vTaskStartScheduler();
	
	while(1);

	return 0;
}


static void Task_Init(void *p_arg)
{
	(void)p_arg;

	initBoard();

	GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);//led off
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);//led off

	ETH_BSP_Config();
	LwIP_Init();

	vTaskDelay(1000);

	xTaskCreate(Task_ROS_M_Init, "ROS_M_Init", 1200, NULL, tskIDLE_PRIORITY, NULL);

	vTaskDelete(NULL);
}


static void Task_ROS_M_Init(void *p_arg)
{
	(void)p_arg;

	ros_m_init(1, NULL, "node_A");
	ros_m_nodeHandle_init();

	ros_m_nodeHandle_serviceClient(ros_m_service_create_service_file("sss", "f41c6983d2c45d8d7a3b231f7161ba85", "int32 a\r\nint32 b\r\n---\r\nint32 c"), "Service_A", 1);

	srv1 s;
	
	s.a = 2;
 	s.b = 3;

	while(1)
	{
		vTaskDelay(3000);

		if(ros_m_service_call("Service_A", (void*)&s))
		{
			printf("res : %d + %d = %d\n", s.a, s.b, s.c);
		}
	}

	vTaskDelete(NULL);
}





