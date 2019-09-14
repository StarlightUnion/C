#include <STC15Wxxxx.H>
#include "pwm.h"
//--------------------------
//  PWM模块初始化/配置
//--------------------------
void PWM_MOUDLE_Init(void)
{
    P_SW2 |= 0x80;                  //使能访问XSFR(扩展RAM区的特殊功能寄存器)
    PWMCFG = 0x00;                  //配置PWM的输出初始电平为低电平
    PWMCKS = 0x0e;                  //选择PWM的时钟为Fosc/(15+1)	24MHz/16=1.5MHz=0.667us
    PWMC   = CYCLE;                 //设置PWM周期  1/1.5*1500=1000us=1ms Motor_Frequence=1KHz
    PWMCR  = 0x0f;			//使能PWM信号输出：PWM2/3/4/5端口为PWM输出，受波形发生器控制
    //控制电机1//
    PWM2T1 = 0x0000;                //设置PWM2第1次反转的PWM计数
    PWM2T2 = 0x0003;                //设置PWM2第2次反转的PWM计数，占空比为(PWM2T2-PWM2T1)/PWMC                                
    PWM2CR = 0x00;                  //选择PWM2输出到P3.7,不使能PWM2中断
    //控制电机2//
    PWM3T1 = 0x0000;                //设置PWM3第1次反转的PWM计数
    PWM3T2 = 0x0003;                //设置PWM3第2次反转的PWM计数，占空比为(PWM3T2-PWM3T1)/PWMC                                
    PWM3CR = 0x00;                  //选择PWM3输出到P2.1,不使能PWM3中断
    //控制电机3//
    PWM4T1 = 0x0000;                //设置PWM4第1次反转的PWM计数
    PWM4T2 = 0x0003;                //设置PWM4第2次反转的PWM计数，占空比为(PWM4T2-PWM4T1)/PWMC                                
    PWM4CR = 0x00;                  //选择PWM4输出到P2.2,不使能PWM4中断
    //控制电机4//
    PWM5T1 = 0x0000;                //设置PWM5第1次反转的PWM计数
    PWM5T2 = 0x0003;                //设置PWM5第2次反转的PWM计数，占空比为(PWM5T2-PWM5T1)/PWMC                                
    PWM5CR = 0x00;                  //选择PWM5输出到P2.3,不使能PWM5中断 
    //总使能//
    PWMCR |= 0x80;                  //使能PWM模块
    P_SW2 &= ~0x80;                 //禁止访问XSFR  	    
}
//-----------------------------
//       设定电机1占空比
//-----------------------------
void SetPWM_M1(long DUTY_1)			
{
    //单路使能//
    P_SW2 |= 0x80; 
    PWMCR  = 0x0f;			//使能PWM信号输出：PWM2/3/4/5端口为PWM输出，受波形发生器控制
    //控制电机1//
    PWM2T1 = 0x0000;                //设置PWM2第1次反转的PWM计数
    PWM2T2 = DUTY_1;                //设置PWM2第2次反转的PWM计数，占空比为(PWM2T2-PWM2T1)/PWMC                                
    PWM2CR = 0x00;                  //选择PWM2输出到P3.7,不使能PWM2中断
    //总使能//
    PWMCR |= 0x80;                  //使能PWM模块
    P_SW2 &= ~0x80;
}
//-----------------------------
//       设定电机2占空比
//-----------------------------
void SetPWM_M2(long DUTY_2)			
{
    //单路使能//
    P_SW2 |= 0x80; 
    PWMCR  = 0x0f;
    //控制电机2//
    PWM3T1 = 0x0000;                //设置PWM3第1次反转的PWM计数
    PWM3T2 = DUTY_2;                //设置PWM3第2次反转的PWM计数，占空比为(PWM3T2-PWM3T1)/PWMC                                
    PWM3CR = 0x00;                  //选择PWM3输出到P2.1,不使能PWM3中断
    //总使能//
    PWMCR |= 0x80;                  //使能PWM模块
    P_SW2 &= ~0x80; 
}
//-----------------------------
//       设定电机3占空比
//-----------------------------
void SetPWM_M3(long DUTY_3)			
{
    //单路使能//
    P_SW2 |= 0x80; 
    PWMCR  = 0x0f;
    //控制电机3//
    PWM4T1 = 0x0000;                //设置PWM4第1次反转的PWM计数
    PWM4T2 = DUTY_3;   			//设置PWM4第2次反转的PWM计数，占空比为(PWM4T2-PWM4T1)/PWMC                                
    PWM4CR = 0x00;                  //选择PWM4输出到P2.2,不使能PWM4中断
    //总使能//
    PWMCR |= 0x80;                  //使能PWM模块
    P_SW2 &= ~0x80; 
}
//-----------------------------
//       设定电机4占空比
//-----------------------------
void SetPWM_M4(long DUTY_4)			
{
    //单路使能//
    P_SW2 |= 0x80; 
    PWMCR  = 0x0f;
    //控制电机4//
    PWM5T1 = 0x0000;                //设置PWM5第1次反转的PWM计数
    PWM5T2 = DUTY_4;  		      //设置PWM5第2次反转的PWM计数，占空比为(PWM5T2-PWM5T1)/PWMC                                
    PWM5CR = 0x00;                  //选择PWM5输出到P2.3,不使能PWM5中断
    //总使能//
    PWMCR |= 0x80;                  //使能PWM模块
    P_SW2 &= ~0x80; 
}