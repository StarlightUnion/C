#ifndef _ANGLE_H_
#define _ANGLE_H_
//////////////////////////////////////////////////
#define  S2RI  0x01                      //S2CON.0
#define  S2TI  0x02                      //S2CON.1
#define  S3RI  0x01              	     //S3CON.0
#define  S3TI  0x02              	     //S3CON.1

extern  bit   Screen_flag;	           //屏幕刷新使能标志位
extern  uchar Screen_count;		     //屏幕刷新计数值

void    SendData(uchar dat);
void    SendString(char *s);
void    Uart_Init(void);
void    GPIO_Init(void);
//////////////////////////////////////////////////
#endif