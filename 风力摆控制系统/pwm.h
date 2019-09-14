#ifndef _PWM_H_
#define _PWM_H_

#define CYCLE    1500L     		//定义PWM周期(最大值为32767)
	
void SetPWM_M1(long DUTY_1);
void SetPWM_M2(long DUTY_2);
void SetPWM_M3(long DUTY_3);
void SetPWM_M4(long DUTY_4);
void PWM_MOUDLE_Init(void);

#endif
