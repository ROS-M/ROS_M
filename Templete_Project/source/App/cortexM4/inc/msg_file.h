#ifndef MSG_FILE_H
#define MSG_FILE_H


typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;


typedef struct Twist
{
	float64 linear_x;
	float64 linear_y;
	float64 linear_z;
	float64 angular_x;
	float64 angular_y;
	float64 angular_z;
} msg1;


typedef struct MotorPower
{
	uint8 data1;
} msg2;


typedef struct int32_8
{
	int32 data1;
	int32 data2;
	int32 data3;
	int32 data4;
	int32 data5;
	int32 data6;
	int32 data7;
	int32 data8;
} msg3;


typedef struct int32_1
{
	int32 data1;
} msg4;


#endif
