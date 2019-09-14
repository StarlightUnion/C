#include <STC15Wxxxx.H>
#include <stdio.h>
#include "comm.h"
#include "angle.h"
#include "key.h"

uint   CurMode = 1;	       //ִ������
bit    RoundDir = 0;        	 //Բ���˶�����ת����
float  R = 20.0; 		       //�ڶ��뾶(cm)
float  angle = 0.0;	       //�ڶ��Ƕȣ������ʣ�
//----------------------------
//      Ӳ���ӿڶ���
//----------------------------
sbit Key_ModeChange = P1^7;	 //ģʽ�л�	   ����Ŀ�л���
sbit Key_TurnChange = P2^0;	 //ת���л�	   ��Բ���˶���
sbit Key_AngleUp    = P1^6;	 //�ڶ�������  �����ڡ�Բ���˶���
sbit Key_AngleDown  = P1^5;	 //�ڶ����ȼ�
sbit Key_DistUp     = P1^4;	 //�ڶ��Ƕ���  ������ǵ����˶���
sbit Key_DistDown   = P1^3;	 //�ڶ��Ƕȼ�
//----------------------------
//ϵͳͨ����ʱ  @24.000MHz 1ms
//----------------------------
void Delay1ms(void)
{
	uchar i, j;
	i = 24;
	j = 85;
	do{ while (--j);} while (--i);
} 
//----------------------------
//      ����ɨ�躯��
//----------------------------
void Key_Scan(void)
{
	/*��Ŀ�л�*/
	if(0 == Key_ModeChange)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_ModeChange)
		{
			CurMode ++;              	         //��Ŀ��������
			if(CurMode > 6)  CurMode = 1;          //�����Ŀ6
			switch(CurMode)
			{
				case(1):  SendString("DS24(105,45,'����Ҫ�� 1',4,0);\r\n"); break;
				case(2):  SendString("DS24(105,45,'����Ҫ�� 2',4,0);\r\n"); break;
				case(3):  SendString("DS24(105,45,'����Ҫ�� 3',4,0);\r\n"); break;
				case(4):  SendString("DS24(105,45,'����Ҫ�� 4',4,0);\r\n"); break;
				case(5):  SendString("DS24(105,45,'����Ҫ�� 1',4,0);\r\n"); break;
				case(6):  SendString("DS24(105,45,'����Ҫ�� 2',4,0);\r\n"); break;
				default:  break;
			}	
		}
		while(0 == Key_ModeChange);			
	}
	/*ת���л�*/
	if(0 == Key_TurnChange)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_TurnChange)	
		{
			RoundDir =~ RoundDir;
			if(RoundDir)  SendString("DS16(215,205,'��ʱ��',5,0);\r\n");
			else          SendString("DS16(215,205,'˳ʱ��',5,0);\r\n");	
		}
		while(0 == Key_TurnChange);
	}
	/*�ڷ�����,�����˶���Բ���˶�������*/
	if(0 == Key_DistUp)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_DistUp)
		{
			R ++;					 //�ڶ���������
			if(R > 35) R = 35;             //�ڷ�����35cm
			sprintf(uart3_buf2,"DS16(210,145,' %.1f cm',5,0);\r\n",R*2); 
			SendString(uart3_buf2);			 //�ڷ�������ʾ
		}
		while(0 == Key_DistUp);
	}
	if(0 == Key_DistDown)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_DistDown)
		{
			R --;					 //�ڶ������Լ�
			if(R < 10) R = 10;             //�ڷ�����10cm
			sprintf(uart3_buf2,"DS16(210,145,' %.1f cm',5,0);\r\n",R*2); 
			SendString(uart3_buf2);			 //�ڷ�������ʾ
		}
		while(0 == Key_DistDown);
	}
	/*�ڽ����ã������ڵ�����*/
	if(0 == Key_AngleUp)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_AngleUp)
		{
			angle += 30;			 //�ڶ��Ƕ���30
			if(angle > 180) angle = 180;   //�ڽ�����180��
			sprintf(uart3_buf2,"DS16(210,175,' %.1f�� ',5,0);\r\n",angle); 
			SendString(uart3_buf2);			 //�ڽǲ�����ʾ
		}		
		while(0 == Key_AngleUp);
	}
	if(0 == Key_AngleDown)
	{
		Delay1ms();Delay1ms();
		if(0 == Key_AngleDown)
		{
			angle -= 30;			 //�ڶ��Ƕȼ�30
			if(angle <= 0)   angle = 0;    //�ڽ�����0��
			sprintf(uart3_buf2,"DS16(210,175,' %.1f�� ',5,0);\r\n",angle); 
			SendString(uart3_buf2);			 //�ڽǲ�����ʾ
		}
		while(0 == Key_AngleDown);
	}
}