#include <STC15Wxxxx.H>
#include <stdio.h>
#include "comm.h"
#include "angle.h"
#include "key.h"

uint   CurMode = 1;	       //执行问题
bit    RoundDir = 0;        	 //圆周运动正反转控制
float  R = 20.0; 		       //摆动半径(cm)
float  angle = 0.0;	       //摆动角度（第三问）
//----------------------------
//      硬件接口定义
//----------------------------
sbit Key_ModeChange = P1^7;	 //模式切换	   （题目切换）
sbit Key_TurnChange = P2^0;	 //转向切换	   （圆周运动）
sbit Key_AngleUp    = P1^6;	 //摆动幅度增  （单摆、圆周运动）
sbit Key_AngleDown  = P1^5;	 //摆动幅度减
sbit Key_DistUp     = P1^4;	 //摆动角度增  （任意角单摆运动）
sbit Key_DistDown   = P1^3;	 //摆动角度减
//----------------------------
//系统通用延时  @24.000MHz 1ms
//----------------------------
void Delay1ms(void)
{
	uchar i, j;
	i = 24;
	j = 85;
	do{ while (--j);} while (--i);
} 
//----------------------------
//      键盘扫描函数
//----------------------------
void Key_Scan(void)
{
	/*题目切换*/
	if(0 == Key_ModeChange)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_ModeChange)
		{
			CurMode ++;              	         //题目变量自增
			if(CurMode > 6)  CurMode = 1;          //最高题目6
			switch(CurMode)
			{
				case(1):  SendString("DS24(105,45,'基本要求 1',4,0);\r\n"); break;
				case(2):  SendString("DS24(105,45,'基本要求 2',4,0);\r\n"); break;
				case(3):  SendString("DS24(105,45,'基本要求 3',4,0);\r\n"); break;
				case(4):  SendString("DS24(105,45,'基本要求 4',4,0);\r\n"); break;
				case(5):  SendString("DS24(105,45,'发挥要求 1',4,0);\r\n"); break;
				case(6):  SendString("DS24(105,45,'发挥要求 2',4,0);\r\n"); break;
				default:  break;
			}	
		}
		while(0 == Key_ModeChange);			
	}
	/*转向切换*/
	if(0 == Key_TurnChange)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_TurnChange)	
		{
			RoundDir =~ RoundDir;
			if(RoundDir)  SendString("DS16(215,205,'逆时针',5,0);\r\n");
			else          SendString("DS16(215,205,'顺时针',5,0);\r\n");	
		}
		while(0 == Key_TurnChange);
	}
	/*摆幅设置,单摆运动和圆周运动都适用*/
	if(0 == Key_DistUp)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_DistUp)
		{
			R ++;					 //摆动幅度自增
			if(R > 35) R = 35;             //摆幅上限35cm
			sprintf(uart3_buf2,"DS16(210,145,' %.1f cm',5,0);\r\n",R*2); 
			SendString(uart3_buf2);			 //摆幅参数显示
		}
		while(0 == Key_DistUp);
	}
	if(0 == Key_DistDown)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_DistDown)
		{
			R --;					 //摆动幅度自减
			if(R < 10) R = 10;             //摆幅下限10cm
			sprintf(uart3_buf2,"DS16(210,145,' %.1f cm',5,0);\r\n",R*2); 
			SendString(uart3_buf2);			 //摆幅参数显示
		}
		while(0 == Key_DistDown);
	}
	/*摆角设置，仅限于第三问*/
	if(0 == Key_AngleUp)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_AngleUp)
		{
			angle += 30;			 //摆动角度增30
			if(angle > 180) angle = 180;   //摆角上限180°
			sprintf(uart3_buf2,"DS16(210,175,' %.1f° ',5,0);\r\n",angle); 
			SendString(uart3_buf2);			 //摆角参数显示
		}		
		while(0 == Key_AngleUp);
	}
	if(0 == Key_AngleDown)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_AngleDown)
		{
			angle -= 30;			 //摆动角度减30
			if(angle <= 0)   angle = 0;    //摆角下限0°
			sprintf(uart3_buf2,"DS16(210,175,' %.1f° ',5,0);\r\n",angle); 
			SendString(uart3_buf2);			 //摆角参数显示
		}
		while(0 == Key_AngleDown);
	}
}