# GUIDE TO 'ROS_M'

## Forder Description

### build Folder
You can use the 'Makefile' to build the source code in the source folder. You can select the target platform (Embedded or PC) using # annotations.

### source Folder
This folder contains the 'ROS_M' library. There are also FreeRTOS and Lwip libraries used on the embedded platforms. The Board folder contains the source code for the board and peripheral settings and implements the application from the source of the App folder.

### ROS_M / Core Folder
The core folder contains the core source code for the 'ROS_M' library.

### ROS_M / Portable Folder
The Portable folder contains interface source code that makes it easy to port the 'ROS_M' library to other platforms.
Currently, you can port 'ROS_M' using FreeRTOS and lwip on an embedded platform, or port it to Linux.




## How To Use It

### Used in Embedded
1. Download vmware workstation player from https://my.vmware.com/en/web/vmware/free#desktop_end_user_computing/vmware_workstation_player/15_0
2. Download linux OS image(Ubuntu 14.04.5 LTS) from https://www.ubuntu.com/download/alternative-downloads and create virtual computer.
3. Refer to http://wiki.ros.org/indigo/Installation/Ubuntu to install ROS Indigo.
4. Prepare 'myCortex-STM32F4' board(I used this board, or prepare a board that can configure Ethernet with PHY chip)
5. Clone this repositories to your local repository.
6. Set the source code in the BSP folder to match the peripheral device.
7. Set the static IP of the target platform in the netconf.c source code in the App folder.
8. Set 'ROS_HOST_IP' and 'ROS_MASTER_IP' appropriately in the ros_m_opts.h source code in the App folder (same as ROS environment variable).
9. Select "SYSTEM := CortexM4" in 'Makefile' by # comment.
10. Build this project.
11. Download .bin file to target embedded system using flash downloader program.
12. Execute in your Embedded platform.

### Used in Linux
1. Download vmware workstation player from https://my.vmware.com/en/web/vmware/free#desktop_end_user_computing/vmware_workstation_player/15_0
2. Download linux OS image(Ubuntu 14.04.5 LTS) from https://www.ubuntu.com/download/alternative-downloads and create virtual computer.
3. Refer to http://wiki.ros.org/indigo/Installation/Ubuntu to install ROS Indigo.
4. Clone this repositories to your local repository.
5. Select "SYSTEM := LINUX" in Makefile by # comment.
6. Build this project.
7. Execute in your PC.




### Example App Source Code
Example source code for node_A that publishes topic_A and subscribes to topic_B.
```
#include "ros_m_portable.h"
#include "msg_file.h"


void callback(msg1* m)
{
	printf("callback : %d, %d\n", m -> data2, m -> data1);
}


int main(int argc, char** argv)
{
	ros_m_init(argc, argv, "node_A");
	ros_m_nodeHandle_init();

	ros_m_nodeHandle_advertise(ros_m_message_create_msg_file("ttt", "d818ecfc1a897d729b61116f6ea3d038", "int8 data1\r\nint32 data2"), "topic_A", 5, 0);
	ros_m_nodeHandle_subscribe(ros_m_message_create_msg_file("ttt", "d818ecfc1a897d729b61116f6ea3d038", "int8 data1\r\nint32 data2"), "topic_B", 5, callback);

	msg1 m;

	m.data1 = 5;
 	m.data2 = 5;
	
	while(ros_m_ok())
	{
		ros_m_spinOnce();
		m.data1++;
 		m.data2++;
		ros_m_nodeHandle_publish("topic_A", (void*)&m);
	}


	return 0;
}

```
