#include <STC15Wxxxx.H>
#include <math.h>
#include "comm.h"
#include "mode.h"
#include "angle.h"
#include "pwm.h"
#include "key.h"
//------------------------------------------
//	         ȫ�ֱ�������				
//------------------------------------------
M1TypeDef  M1;
M2TypeDef  M2;				//��������ṹ��

PIDTypdDef  M1PID;
PIDTypdDef  M2PID;			//PID�����ṹ��

sbit Mode_led = P5^1;       		//����0�жϵĽ��������Ҳ��PID�����ִ�����
sbit Screen_led = P5^2;			//��Ļˢ��ָʾ��
//-----------------------------------------------
//��ʱ��0ΪPID�����ṩ�ȶ����ж�
//�ж�ÿ20ms����һ��
//��ϵͳ�Ĳ�����Ϊ50sa/s  Ƶ��50hz
//-----------------------------------------------
void Timer0_Init(void)		//20����@24.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xC0;			//���ö�ʱ��ֵ
	TH0 = 0x63;			//���ö�ʱ��ֵ
	TF0 = 0;			//���TF0��־
	TR0 = 1;			//��ʱ��0��ʼ��ʱ
}
//-----------------------------------------------
//	 ��ʱ��0�жϷ������	//20����@24.000MHz
//-----------------------------------------------
void Timer0_service()  interrupt 1  
{  	
	Screen_count++;
	if(Screen_count == 10)	    //100ms��ʱʱ�䵽,��Ļ��ˢ��ʱ��Ϊ0.1s
	{
		Screen_led = ~Screen_led;
		Screen_count = 0;
		Screen_flag  = 1;
	} 

	M1.CurPos = Angle_Y; 	   //��ȡ��ǰ�Ƕ�
	M2.CurPos = Angle_X;						   

	switch(CurMode)	         //����CurModeֵѡ��ִ�е�����
	{	
		case 1: Mode_1(); Mode_led =~ Mode_led; break;		 //��һ��
		case 2: Mode_2(); Mode_led =~ Mode_led; break;		 //�ڶ���
		case 3: Mode_3(); Mode_led =~ Mode_led; break;		 //������
		case 4: Mode_4(); Mode_led =~ Mode_led; break;		 //������
		case 5: Mode_5(); Mode_led =~ Mode_led; break;		 //������
		case 6: Mode_6(); Mode_led =~ Mode_led; break;		 //������
		default: break;							 //����������Ҫ��δ��
	}			
}
/*------------------------------------------------*
//��һ�ʵ�ʵ�֣�
//��һ�ʵ�Ҫ������ڵڶ����У�����ڣ�
//���Ƕȷ�ֵ�趨Ϊ23�㣬ת��Ϊ���ȼ�Ϊ60cm	          //���ʲ��ٸ���
//��ĿҪ��ڶ�����Ϊ > 50cm,�ﵽҪ��
//------------------------------------------------*/
void Mode_1(void)		     //�����
{
	const float priod = 1600.0;                  //��������1800ms const:���ɱ�ֵ
	static uint MoveTimeCnt = 0;	               //ʱ���ۼӱ�����ʵ���ϴ˱���ֻ�ڳ���ʼʱ��ʼ��һ��
	float set_y = 0.0;		               //y��Ƕ��趨ֵ����PID�ĸ�������
	float A = 0.0;			               //�ڷ���A����ڶ��Ƕȷ�ֵ,��λ���ȣ������ǰڶ�����ֵ
	float Normalization = 0.0;	               //��һ����
	float Omega = 0.0;		               //���ٶ�	
		
	MoveTimeCnt += 20;		               //20ms�������һ��
	Normalization = (float)MoveTimeCnt / priod;  //�������ڹ�һ������ÿ������ʱʱ���ۼ����Ե������ڵ�ռ��
	Omega = 2.0*3.14159*Normalization;		   //Omega��һ����һ���ڶ������ڵ�
	set_y = 23*sin(Omega);                       //Ŀ��ֵ�����ҹ��ɱ仯
				
	PID_M1_SetPoint(set_y);	                     //M1�趨����
	PID_M1_SetKp(36);				  	   /*----------------------------------------*/
	PID_M1_SetKi(0.198);	 			   /*ÿ��һ�α���������һ�ζ�����PID���ƹ��� */
	PID_M1_SetKd(1100);				   /*----------------------------------------*/
	PID_M2_SetPoint(0);		               //M2�趨����
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 	 	

	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);	   //���1��3 PID����
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);       //���2��4 PID����

	if(M1.PWM > POWER_MAX) M1.PWM  =  POWER_MAX; //����޷�
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX; 	
	if(M2.PWM > POWER_MAX) M2.PWM  =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX;
				
	MotorMove(M1.PWM,M2.PWM);                     //�������	
}
/*------------------------------------------------*
//�ڶ��ʵ�ʵ�֣�
//��һ����������������(2��)���ֳ�90��:1.8s*50Hz=90
//ÿ20msִ��һ��PID�����Ը���Ŀ��ֵ 		     //���ʲ��ٸ���
//��ĿҪ��ڷ���30-60cm֮������ã��ﵽҪ��					    
//------------------------------------------------*/
void Mode_2(void)
{
	const float priod = 1600.0;                  //��������1800ms const:���ɱ�ֵ
	static uint MoveTimeCnt = 0;	               //ʱ���ۼӱ�����ʵ���ϴ˱���ֻ�ڳ���ʼʱ��ʼ��һ��
	float set_y = 0.0;		               //y��Ƕ��趨ֵ����PID�ĸ�������
	float A = 0.0;			               //�ڷ���A����ڶ��Ƕȷ�ֵ,��λ���ȣ������ǰڶ�����ֵ
	float Normalization = 0.0;	               //��һ����
	float Omega = 0.0;		               //���ٶ�	
		
	MoveTimeCnt += 20;		               //20ms�������һ��
	Normalization = (float)MoveTimeCnt / priod;  //�������ڹ�һ������ÿ������ʱʱ���ۼ����Ե������ڵ�ռ��
	Omega = 2.0*3.14159*Normalization;		   //Omega��һ����һ���ڶ������ڵ�
	A = atan((R/70.0f))*57.2958f;                //�ڶ����ȷ�ֵת��Ϊ�ڶ��Ƕȷ�ֵ��84(cm)Ϊ����ڵ���ظ߶ȣ�ʵ�ʵ�������ѡȡ70Ϊ��
	set_y = A*sin(Omega);                        //Ŀ��ֵ�����ҹ��ɱ仯
				
	PID_M1_SetPoint(set_y);	                     //M1�趨����
	PID_M1_SetKp(36);				   	   
	PID_M1_SetKi(0.198);	 			   
	PID_M1_SetKd(1100);			
	PID_M2_SetPoint(0);		               //M2�趨����
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 	 	

	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);	   //���1��3 PID����
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);       //���2��4 PID����

	if(M1.PWM > POWER_MAX) M1.PWM  =  POWER_MAX; //����޷�
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX; 	
	if(M2.PWM > POWER_MAX) M2.PWM  =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX;
				
	MotorMove(M1.PWM,M2.PWM);                     //�������
}
/*------------------------------------------------*
//�����ʵ�ʵ�֣�
//�Ƕȿ����õİڶ���15s֮��ʵ�֣�
//����������ͼ�ϳ�˼�룬X��Y����������ļ�г�˶�
//��������г�˶�����λ��Ϊ0��У���������ֱ�߶�		 //���ʲ��ٸ���
//�ڽ���������г�˶�������йأ�
//tan�� = A2/A1  -- �� = 0
//tan�� = -A2/A1 -- �� = ��
/------------------------------------------------*/
void Mode_3(void)
{
	const float priod = 1600.0;        //��������1800ms const:���ɱ�ֵ
	static uint MoveTimeCnt = 0;	     //ʱ���ۼӱ�����
	float set_x = 0.0;		     //X��Ƕ��趨ֵ����PID�ĸ�������
	float set_y = 0.0;		     //Y��Ƕ��趨ֵ����PID�ĸ�������
	float Ax = 0.0;			     //X��ڷ�����
	float Ay = 0.0;			     //Y��ڷ�����
	float A = 0.0;			     //�ܰڷ���ΪX��Y��ڷ���ʸ����
	float Normalization = 0.0;	     //�������ڹ�һ����
	float Omega = 0.0;		     //�Ƕȹ�һ����
	
	MoveTimeCnt += 20;					 //20ms�������һ��
	Normalization = (float)MoveTimeCnt / priod;	 //�������ڹ�һ������ÿ������ʱʱ���ۼ����Ե������ڵ�ռ��
	Omega = 2.0*3.14159*Normalization;			 //Omega��һ����һ���ڶ������ڵ�
	A = atan((R/60.0f))*57.2958f;                    //�ڶ����ȷ�ֵת��Ϊ�ڶ��Ƕȷ�ֵ��Ҫת��Ϊ�Ƕ�                  						
	Ax = A*cos(angle*0.017453);	                   //����X��ڷ�����������Ϊ����
	Ay = A*sin(angle*0.017453);	                   //����Y��ڷ�����������Ϊ����
	set_x = Ax*sin(Omega); 		                   //����X��Ŀ��ֵ
	set_y = Ay*sin(Omega);            			 //����Y��Ŀ��ֵ
		
	PID_M1_SetPoint(set_y);	                         //M1�趨����
	PID_M1_SetKp(36);				   	   
	PID_M1_SetKi(0.198);	 			   
	PID_M1_SetKd(1100);				   
	PID_M2_SetPoint(set_x);		                   //M2�趨����
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 
	
	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);	       //���1��3 PID����
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);           //���2��4 PID����
	
	if(M1.PWM > POWER_MAX)  M1.PWM =  POWER_MAX;	 //����޷�
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX;		 	
	if(M2.PWM > POWER_MAX)  M2.PWM =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX;		

	MotorMove(M1.PWM,M2.PWM);				 //�������
}
/*------------------------------------------------------*
//�����ʵ�ʵ�֣�
//��X��Y���趨ֵ��Ϊ0��Ȼ��PID�������
//��5s��ʹ��ھ�ֹ							  //���ʲ��ٸ���
//���ʵ�PID�����������ʲ�ͬ
//------------------------------------------------------*/
void Mode_4(void)
{
	if(abs(M1.CurPos)<40.0 && abs(M2.CurPos)<40.0)	     //X��Y��Ƕ�С��40����ƶ�
	{		
		PID_M1_SetPoint(0);	                       //M1�趨����
		PID_M1_SetKp(18);				   
		PID_M1_SetKi(0.01);	 			   
		PID_M1_SetKd(2600);				   
		PID_M2_SetPoint(0);		                 //M2�趨����
		PID_M2_SetKp(18);    
		PID_M2_SetKi(0.01);		
		PID_M2_SetKd(2600);
			
		M1.PWM = PID_M1_PosLocCalc(M1.CurPos);         //���1��3 PID����
		M2.PWM = PID_M2_PosLocCalc(M2.CurPos);         //���2��4 PID����
		
		if(M1.PWM > POWER_MAX)  M1.PWM = 1300;
		if(M1.PWM < -POWER_MAX) M1.PWM = -1300;
	
		if(M2.PWM > POWER_MAX)  M2.PWM = 1300;
		if(M2.PWM < -POWER_MAX) M2.PWM = -1300;        //����޷�
	}
	else	
	{
	 	M1.PWM = 10L;
		M2.PWM = 10L;		                       //�Ƕȹ���ͣ��
	}	
	MotorMove(M1.PWM,M2.PWM);	                       //�������
}
/*-----------------------------------------------------*
//�����ʵ�ʵ�֣�
//�뾶�����õ�Բ���˶�
//��������г�˶�����λ��Ϊ��/2���/2����������ֱ�߶�		//���ʲ��ٸ���
// �� = ��/2  ����ʱ��ת
// �� = 3��/2 ��˳ʱ��ת
//-----------------------------------------------------*/	
void Mode_5(void)
{
	const float priod = 1600.0;                  //��������1600ms 
	static uint MoveTimeCnt = 0;
	float set_x = 0.0;
	float set_y = 0.0;
	float A = 0.0;
	float phase = 0.0;
	float Normalization = 0.0;
	float Omega = 0.0;
	
	MoveTimeCnt += 20;				   
	Normalization = (float)MoveTimeCnt / priod;  
	Omega = 2.0*3.14159*Normalization;		   				
	A = atan((R/65.0f))*57.2958f;                
	
	if(RoundDir == 0)  phase = 3.141592/2.0;     //��ʱ����ת��λ��90�� ����/2��	  			         
	else   phase = (3.0*3.141592)/2.0;	         //˳ʱ����ת��λ��270�㣨��/2��
	
	set_x = A*sin(Omega);			         //����X��ڷ�����������Ϊ����
	set_y = A*sin(Omega+phase); 	               //����Y��ڷ�����������Ϊ����
	 
	PID_M1_SetPoint(set_y);	                     //M1�趨����
	PID_M1_SetKp(36);				   	   
	PID_M1_SetKi(0.198);	 			   
	PID_M1_SetKd(1100);				   
	PID_M2_SetPoint(set_x);		               //M2�趨����
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 		 
	
	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);       //���1��3 PID����
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);       //���2��4 PID����
	
	if(M1.PWM > POWER_MAX)  M1.PWM =  POWER_MAX;
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX;
			 	
	if(M2.PWM > POWER_MAX)  M2.PWM =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX; //����޷�		

	MotorMove(M1.PWM,M2.PWM);			   //�������
}
/*------------------------------------------------*
//�����ʵ�ʵ�֣�
//��Բ������
//�����ڵ�������
//------------------------------------------------*/
void Mode_6(void)
{
	const float priod = 1600.0;                  //��������1600ms 
	static uint MoveTimeCnt = 0;
	float set_x = 0.0;
	float set_y = 0.0;
	float A = 0.0;
	float phase = 0.0;
	float Normalization = 0.0;
	float Omega = 0.0;
	
	MoveTimeCnt += 20;				   
	Normalization = (float)MoveTimeCnt / priod;  
	Omega = 2.0*3.14159*Normalization;		   				
	A = atan((R/65.0f))*57.2958f;                
	
	if(RoundDir == 0)  phase = 3.141592/2.0;     //��ʱ����ת��λ��90�� ����/2��	  			         
	else   phase = (3.0*3.141592)/2.0;	         //˳ʱ����ת��λ��270�㣨��/2��
	
	set_x = A*sin(Omega);			         //����X��ڷ�����������Ϊ����
	set_y = A*sin(Omega+phase); 	               //����Y��ڷ�����������Ϊ����
	 
	PID_M1_SetPoint(set_y);	                     //M1�趨����
	PID_M1_SetKp(36);				   	   
	PID_M1_SetKi(0.198);	 			   
	PID_M1_SetKd(1100);				   
	PID_M2_SetPoint(set_x);		               //M2�趨����
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 		 
	
	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);       //���1��3 PID����
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);       //���2��4 PID����
	
	if(M1.PWM > POWER_MAX)  M1.PWM =  POWER_MAX;
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX;
			 	
	if(M2.PWM > POWER_MAX)  M2.PWM =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX; //����޷�		

	MotorMove(M1.PWM,M2.PWM);			   //�������
}
//------------------------------------------
//       ���1PID�����ṹ���ʼ��
//   ����ĵ��1����ʵ�ʵĵ��1�ź�3��		
//------------------------------------------
void PID_M1_Init(void)
{							//M1
	M1PID.LastError  = 0;			//Error[-1]
	M1PID.PrevError  = 0;			//Error[-2]
	M1PID.Proportion = 0;			//Proportional
	M1PID.Integral   = 0;			//Integral 
	M1PID.Derivative = 0;			//Derivative 
	M1PID.SetPoint   = 0;
	M1PID.SumError   = 0;
}
//------------------------------------------
//       ���2PID�����ṹ���ʼ��
//  	����ĵ��1����ʵ�ʵĵ��2�ź�4��	
//------------------------------------------
void PID_M2_Init(void)
{							//M2
	M2PID.LastError  = 0;			//Error[-1]
	M2PID.PrevError  = 0;			//Error[-2]
	M2PID.Proportion = 0;			//Proportional Const
	M2PID.Integral   = 0;			//Integral Const
	M2PID.Derivative = 0;			//Derivative Const
	M2PID.SetPoint   = 0;
	M2PID.SumError   = 0;
}
//------------------------------------------
//          ���1PID�����趨		
//------------------------------------------
void PID_M1_SetPoint(float setpoint)
{	
	M1PID.SetPoint = setpoint;		//�� �� Ŀ��ֵ��M1��M3
}
void PID_M1_SetKp(float dKpp)
{	
	M1PID.Proportion = dKpp;		//�趨����ϵ����M1��M3
}
void PID_M1_SetKi(float dKii)
{	
	M1PID.Integral = dKii;			//�趨����ϵ����M1��M3
}
void PID_M1_SetKd(float dKdd)
{	
	M1PID.Derivative = dKdd;		//�趨΢��ϵ����M1��M3
}
//------------------------------------------
//	      ���2PID�����趨
//------------------------------------------
void PID_M2_SetPoint(float setpoint)
{	
	M2PID.SetPoint = setpoint;		//�� �� Ŀ��ֵ��M2��M4
}
void PID_M2_SetKp(float dKpp)
{	
	M2PID.Proportion = dKpp;		//�趨����ϵ����M2��M4
}
void PID_M2_SetKi(float dKii)
{	
	M2PID.Integral = dKii;			//�趨����ϵ����M2��M4
}
void PID_M2_SetKd(float dKdd)
{	
	M2PID.Derivative = dKdd;		//�趨΢��ϵ����M2��M4
}
//------------------------------------------
//���ܣ����1λ��ʽPID����
//����˵����*****
//	��ڲ�������ڵ�ǰλ�ã�Y��Ƕȣ�
//    ���ڲ��������1��3��PWM��
//------------------------------------------
int PID_M1_PosLocCalc(float NextPoint)
{
      float  iError,dError;   
	iError = M1PID.SetPoint - NextPoint;        //��ǰ��� = �趨�Ƕ� - ʵ�ʽǶ�
	M1PID.SumError += iError;			  //����ۻ������֣�
	if(M1PID.SumError > 2300.0)			  //�����޷�
		M1PID.SumError = 2300.0;
	else if(M1PID.SumError < -2300.0)
		M1PID.SumError = -2300.0;	
	dError = iError - M1PID.LastError; 		  //΢��
	M1PID.LastError = iError;			  //������
	return(int)  (  M1PID.Proportion   * iError          	//��ɢPID���� 
          		    + M1PID.Integral   * M1PID.SumError 	 
          		    + M1PID.Derivative * dError);
}
//------------------------------------------
//���ܣ����2λ��ʽPID����
//����˵����*****
//    ��ڲ�������ڵ�ǰλ�ã�X��Ƕȣ�
//    ���ڲ��������2��4��PWM��		
//------------------------------------------
int PID_M2_PosLocCalc(float NextPoint)
{
	float  iError,dError;
	iError = M2PID.SetPoint - NextPoint;        //��ǰ��� = �趨�Ƕ� - ʵ�ʽǶ�
	M2PID.SumError += iError;			  //����ۻ������֣�
	if(M2PID.SumError > 2300.0)			  //�����޷�
		M2PID.SumError = 2300.0;
	else if(M2PID.SumError < -2300.0)
		M2PID.SumError = -2300.0;
	dError = iError - M2PID.LastError; 		  //΢��
	M2PID.LastError = iError;			  //������	
	return(int)(  M2PID.Proportion   * iError           	
          		  + M2PID.Integral   * M2PID.SumError 	//��ɢPID���� 
          		  + M2PID.Derivative * dError);
}
//------------------------------------------
//���ܣ�����ײ���������
//����˵����
//pwm1�����1��3��������Y���˶���Roll��
//pwm2: ���2��4��������X���˶���pitch��
//���ڲ�������
//------------------------------------------
void MotorMove(long pwm1,long pwm2)
{
	if(pwm1 > 0)			      /*��������Ƕȿ�ʼ���*/
	{						/*����ڶ����Ƕ�Ϊ���������Ϊ����pid������(pwm1)Ϊ��*/
	 	SetPWM_M1(1L);		      /*���Ұڶ����Ƕ�Ϊ���������Ϊ����pid������(pwm1)Ϊ��*/
		SetPWM_M3(pwm1);	      	/*pwmΪ������ʾ�����ڣ����3���������1ͣ��*/
	}						/*pwmΪ������ʾ����Ұڣ����1���������3ͣ��*/
	else if(pwm1 < 0)				/*Ӳ�������*/
	{						/*�������ڣ�Y��Ƕ�Ϊ��*/
	 	SetPWM_M1(abs(pwm1));		/*������ڣ�X��Ƕ�Ϊ��*/
		SetPWM_M3(1L);			/*��֮*/
	}						/*���ֻ�ɶ�����ͣ�����޷�ת����*/
	if(pwm2 > 0)
	{
	 	SetPWM_M4(1L);		    	//���4ͣ��
		SetPWM_M2(pwm2);			//���2����
	}
	else if(pwm2 < 0)
	{
	 	SetPWM_M4(abs(pwm2));		//���4����
		SetPWM_M2(1L);			//���2ͣ��
	} 	
}