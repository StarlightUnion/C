#ifndef _MODE1_H_
#define _MODE1_H_
////////////////////////////////////////////////////////////////////
#define POWER_MAX  1000     //������PWM����1000.����1500.
//------------------------------------------
// 	          ��������ṹ��				
//------------------------------------------
typedef struct
{
	float CurPos;			  //λ��ֵ
	volatile int   PWM;	        //PWM	
} M1TypeDef,M2TypeDef;
//------------------------------------------
// 		  PID�����ṹ��				
//------------------------------------------
typedef struct
{
	float  SetPoint; 	      //�趨ֵ
	float  SumError;		//����ۼ�
		
	float  Proportion;      //���� 
	float  Integral;        //����
	float  Derivative;      //΢��

	float  LastError;        // Error[-1]
	float  PrevError;        // Error[-2]
} PIDTypdDef;

extern xdata M1TypeDef  M1;
extern xdata M2TypeDef  M2;		//��������ṹ��
//------------------------
//       ��������
//------------------------
void Timer0_Init(void);		//20����@24.000MHz

void Mode_2(void);
void Mode_1(void);
void Mode_3(void);
void Mode_4(void);
void Mode_5(void);
void Mode_6(void);

void PID_M1_Init(void);
void PID_M2_Init(void);

void PID_M1_SetPoint(float setpoint);
void PID_M1_SetKd(float dKdd);
void PID_M1_SetKi(float dKii);
void PID_M1_SetKp(float dKpp);

void PID_M2_SetPoint(float setpoint);
void PID_M2_SetKd(float dKdd);
void PID_M2_SetKi(float dKii);
void PID_M2_SetKp(float dKpp);

int  PID_M1_PosLocCalc(float NextPoint);
int  PID_M2_PosLocCalc(float NextPoint);

void MotorMove(long pwm1,long pwm2);
////////////////////////////////////////////////////////////
#endif