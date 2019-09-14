//-------------------------------------------------------
//	               风力摆控制系统
//
//      主控：IAP15W4K58S4  |   时钟：24MHz
//	  工作电压：3.3~5v    |   18650*2并联
//      电机：720空心杯*4   |   桨：75mm*4
//	  传感器：MPU6050     |   屏幕：UART串口屏    
//-------------------------------------------------------
#include <STC15Wxxxx.H>
#include <stdio.h>
#include <math.h>
#include "comm.h"
#include "angle.h"
#include "pwm.h"
#include "mode.h"
#include "key.h"
//---------------------------------------------
//	           全局变量定义
//---------------------------------------------
uchar  uart3_buf[35];	           //串口屏待发送数据缓冲区
uchar  uart3_buf2[35];
float  Angle_X,Angle_Y;            //X、Y轴的角度
//----------------------------------------------
//                 main函数
//----------------------------------------------
void main()
{
	GPIO_Init();		   //IO口初始化
	Uart_Init();		   //串口初始化
      SendString("CLS(0);DS16(105,15,'风力摆控制系统',2,0);DS16(40,85,'X轴角度：',15,0);DS16(40,115,'Y轴角度：',15,0);DS16(40,145,'摆动幅度：',15,0);DS16(40,175,'摆动角度：',15,0);DS16(40,205,'圆周方向',15,0);PL(300,75,20,75,1);PL(300,110,20,110,1);PL(300,140,20,140,1);PL(300,170,20,170,1);PL(300,200,20,200,1);PL(300,230,20,230,1);PL(20,75,20,230,1);PL(300,75,300,230,1);PL(160,75,160,230,1););\r\n");
	Timer0_Init();		   //定时器0初始化
	PWM_MOUDLE_Init();	   //PWM模组初始化
	PID_M1_Init();		   //电机1PID参数初始化
	PID_M2_Init();		   //电机2PID参数初始化
	
	ET0 = 1;		     	   //开定时器0中断
	IE2 |= 0x01;		   //开串口2中断

	while(1)
	{		
		if(Screen_flag == 1)	     //角度刷新
		{
			Screen_flag = 0;
			sprintf(uart3_buf,"DS16(210,85,' %.2f° ',5,0);\r\n",Angle_X); SendString(uart3_buf);			
			sprintf(uart3_buf,"DS16(210,115,' %.2f° ',5,0);\r\n",Angle_Y); SendString(uart3_buf);			
		}
		Key_Scan();		           //键盘扫描
	}	
}