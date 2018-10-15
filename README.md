# GUIDE TO ROS_M

## Forder Description

### Core Folder
The core folder contains the core source code for the ROS_M library.

### Portable Folder
The Portable folder contains interface source code that makes it easy to port the ROS_M library to other platforms.
Currently, you can port ROS_M using FreeRTOS and lwip on an embedded platform, or port it to Linux.




## How To Use It

### Used in Embedded
1. 
2. 
3. 

### Used in Linux
1. Download vmware workstation player from https://my.vmware.com/en/web/vmware/free#desktop_end_user_computing/vmware_workstation_player/15_0
2. Download linux OS image(Ubuntu 14.04.5 LTS) from https://www.ubuntu.com/download/alternative-downloads and create virtual computer.
3. Refer to http://wiki.ros.org/indigo/Installation/Ubuntu to install ROS Indigo.
4. Build ROS_M with FreeRTOS and Lwip.
5. Excute ROS node.




### Source Code Example

```
#include "ros_m_portable.h"
#include "msg_file.h"
#include "srv_file.h"


void callback(msg1* m)
{
	printf("callback : %d, %d\n", m -> data2, m -> data1);
}

void callback2(srv1* s)
{
	//printf("callback\n");
	printf("callback : %d, %d\n", s -> a, s -> b);
	s -> c = s -> a + s -> b;
	//sleep(3);
}

int main(int argc, char** argv)
{
	ros_m_init(argc, argv, "node");
	ros_m_nodeHandle_init();

	ros_m_nodeHandle_advertise(ros_m_message_create_message_file("ttt", "d818ecfc1a897d729b61116f6ea3d038", "int8 data1\r\nint32 data2"), "topic_A", 5, 0);
	//ros_m_nodeHandle_subscribe(ros_m_message_create_message_file("ttt", "d818ecfc1a897d729b61116f6ea3d038", "int8 data1\r\nint32 data2"), "topic_A", 5, callback);
	//ros_m_nodeHandle_advertiseService(ros_m_service_create_service_file("sss", "f41c6983d2c45d8d7a3b231f7161ba85", "int32 a\r\nint32 b\r\n---\r\nint32 c"), "service_A", callback2);
	//ros_m_nodeHandle_serviceClient(ros_m_service_create_service_file("sss", "f41c6983d2c45d8d7a3b231f7161ba85", "int32 a\r\nint32 b\r\n---\r\nint32 c"), "service_A");

	msg1 m;

	m.data1 = 5;
 	m.data2 = 5;

	srv1 s;
	
	s.a = 2;
 	s.b = 3;
	//if(ros_m_service_call("service_A", (void*)&s))
	//{
	//	printf("response : %d + %d = %d\n", s.a, s.b, s.c);
	//}


	//ros_m_spin();

	while(ros_m_ok())
	{
		//if(ros_m_service_call("service_A", (void*)&s))
		//{
		//	printf("response : %d + %d = %d\n", s.a, s.b, s.c);
		//}
		//sleep(1);
		m.data1++;
 		m.data2++;
		ros_m_nodeHandle_publish("topic_A", (void*)&m);
	}



	return 0;
}

```
