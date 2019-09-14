#include <STC15Wxxxx.H>
#include <math.h>
#include "comm.h"
#include "mode.h"
#include "angle.h"
#include "pwm.h"
#include "key.h"
//------------------------------------------
//	         全局变量定义				
//------------------------------------------
M1TypeDef  M1;
M2TypeDef  M2;				//电机参数结构体

PIDTypdDef  M1PID;
PIDTypdDef  M2PID;			//PID参数结构体

sbit Mode_led = P5^1;       		//串口0中断的进入情况，也即PID运算的执行情况
sbit Screen_led = P5^2;			//屏幕刷新指示灯
//-----------------------------------------------
//定时器0为PID计算提供稳定的中断
//中断每20ms产生一次
//即系统的采样率为50sa/s  频率50hz
//-----------------------------------------------
void Timer0_Init(void)		//20毫秒@24.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xC0;			//设置定时初值
	TH0 = 0x63;			//设置定时初值
	TF0 = 0;			//清除TF0标志
	TR0 = 1;			//定时器0开始计时
}
//-----------------------------------------------
//	 定时器0中断服务程序	//20毫秒@24.000MHz
//-----------------------------------------------
void Timer0_service()  interrupt 1  
{  	
	Screen_count++;
	if(Screen_count == 10)	    //100ms计时时间到,屏幕的刷新时间为0.1s
	{
		Screen_led = ~Screen_led;
		Screen_count = 0;
		Screen_flag  = 1;
	} 

	M1.CurPos = Angle_Y; 	   //获取当前角度
	M2.CurPos = Angle_X;						   

	switch(CurMode)	         //根据CurMode值选择执行的问题
	{	
		case 1: Mode_1(); Mode_led =~ Mode_led; break;		 //第一问
		case 2: Mode_2(); Mode_led =~ Mode_led; break;		 //第二问
		case 3: Mode_3(); Mode_led =~ Mode_led; break;		 //第三问
		case 4: Mode_4(); Mode_led =~ Mode_led; break;		 //第四问
		case 5: Mode_5(); Mode_led =~ Mode_led; break;		 //第五问
		case 6: Mode_6(); Mode_led =~ Mode_led; break;		 //第六问
		default: break;							 //第三个发挥要求未做
	}			
}
/*------------------------------------------------*
//第一问的实现：
//第一问的要求包含于第二问中（简单起摆）
//将角度幅值设定为23°，转换为长度即为60cm	          //此问不再更改
//题目要求摆动长度为 > 50cm,达到要求
//------------------------------------------------*/
void Mode_1(void)		     //待添加
{
	const float priod = 1600.0;                  //单摆周期1800ms const:不可变值
	static uint MoveTimeCnt = 0;	               //时间累加变量。实际上此变量只在程序开始时初始化一次
	float set_y = 0.0;		               //y轴角度设定值（即PID的跟踪量）
	float A = 0.0;			               //摆幅（A代表摆动角度幅值,单位：度），不是摆动幅度值
	float Normalization = 0.0;	               //归一化量
	float Omega = 0.0;		               //角速度	
		
	MoveTimeCnt += 20;		               //20ms计算调整一次
	Normalization = (float)MoveTimeCnt / priod;  //单摆周期归一化：即每次运算时时间累计量对单摆周期的占比
	Omega = 2.0*3.14159*Normalization;		   //Omega归一化，一个摆动周期内的
	set_y = 23*sin(Omega);                       //目标值呈正弦规律变化
				
	PID_M1_SetPoint(set_y);	                     //M1设定参数
	PID_M1_SetKp(36);				  	   /*----------------------------------------*/
	PID_M1_SetKi(0.198);	 			   /*每进一次本函数都是一次独立的PID控制过程 */
	PID_M1_SetKd(1100);				   /*----------------------------------------*/
	PID_M2_SetPoint(0);		               //M2设定参数
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 	 	

	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);	   //电机1、3 PID运算
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);       //电机2、4 PID运算

	if(M1.PWM > POWER_MAX) M1.PWM  =  POWER_MAX; //输出限幅
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX; 	
	if(M2.PWM > POWER_MAX) M2.PWM  =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX;
				
	MotorMove(M1.PWM,M2.PWM);                     //电机动作	
}
/*------------------------------------------------*
//第二问的实现：
//将一个完整的正弦周期(2π)划分成90份:1.8s*50Hz=90
//每20ms执行一次PID计算以跟踪目标值 		     //此问不再更改
//题目要求摆幅在30-60cm之间可设置，达到要求					    
//------------------------------------------------*/
void Mode_2(void)
{
	const float priod = 1600.0;                  //单摆周期1800ms const:不可变值
	static uint MoveTimeCnt = 0;	               //时间累加变量。实际上此变量只在程序开始时初始化一次
	float set_y = 0.0;		               //y轴角度设定值（即PID的跟踪量）
	float A = 0.0;			               //摆幅（A代表摆动角度幅值,单位：度），不是摆动幅度值
	float Normalization = 0.0;	               //归一化量
	float Omega = 0.0;		               //角速度	
		
	MoveTimeCnt += 20;		               //20ms计算调整一次
	Normalization = (float)MoveTimeCnt / priod;  //单摆周期归一化：即每次运算时时间累计量对单摆周期的占比
	Omega = 2.0*3.14159*Normalization;		   //Omega归一化，一个摆动周期内的
	A = atan((R/70.0f))*57.2958f;                //摆动长度幅值转换为摆动角度幅值，84(cm)为万向节点离地高度，实际调试下来选取70为佳
	set_y = A*sin(Omega);                        //目标值呈正弦规律变化
				
	PID_M1_SetPoint(set_y);	                     //M1设定参数
	PID_M1_SetKp(36);				   	   
	PID_M1_SetKi(0.198);	 			   
	PID_M1_SetKd(1100);			
	PID_M2_SetPoint(0);		               //M2设定参数
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 	 	

	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);	   //电机1、3 PID运算
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);       //电机2、4 PID运算

	if(M1.PWM > POWER_MAX) M1.PWM  =  POWER_MAX; //输出限幅
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX; 	
	if(M2.PWM > POWER_MAX) M2.PWM  =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX;
				
	MotorMove(M1.PWM,M2.PWM);                     //电机动作
}
/*------------------------------------------------*
//第三问的实现：
//角度可设置的摆动（15s之内实现）
//利用李萨如图合成思想，X、Y轴两个方向的简谐运动
//若两个简谐运动的相位差为0或π，则曲线是直线段		 //此问不再更改
//摆角于两个简谐运动的振幅有关：
//tanθ = A2/A1  -- ψ = 0
//tanθ = -A2/A1 -- ψ = π
/------------------------------------------------*/
void Mode_3(void)
{
	const float priod = 1600.0;        //单摆周期1800ms const:不可变值
	static uint MoveTimeCnt = 0;	     //时间累加变量。
	float set_x = 0.0;		     //X轴角度设定值（即PID的跟踪量）
	float set_y = 0.0;		     //Y轴角度设定值（即PID的跟踪量）
	float Ax = 0.0;			     //X轴摆幅分量
	float Ay = 0.0;			     //Y轴摆幅分量
	float A = 0.0;			     //总摆幅：为X、Y轴摆幅的矢量和
	float Normalization = 0.0;	     //单摆周期归一化量
	float Omega = 0.0;		     //角度归一化量
	
	MoveTimeCnt += 20;					 //20ms计算调整一次
	Normalization = (float)MoveTimeCnt / priod;	 //单摆周期归一化：即每次运算时时间累计量对单摆周期的占比
	Omega = 2.0*3.14159*Normalization;			 //Omega归一化，一个摆动周期内的
	A = atan((R/60.0f))*57.2958f;                    //摆动长度幅值转换为摆动角度幅值，要转换为角度                  						
	Ax = A*cos(angle*0.017453);	                   //计算X轴摆幅分量，换算为弧度
	Ay = A*sin(angle*0.017453);	                   //计算Y轴摆幅分量，换算为弧度
	set_x = Ax*sin(Omega); 		                   //计算X轴目标值
	set_y = Ay*sin(Omega);            			 //计算Y轴目标值
		
	PID_M1_SetPoint(set_y);	                         //M1设定参数
	PID_M1_SetKp(36);				   	   
	PID_M1_SetKi(0.198);	 			   
	PID_M1_SetKd(1100);				   
	PID_M2_SetPoint(set_x);		                   //M2设定参数
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 
	
	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);	       //电机1、3 PID运算
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);           //电机2、4 PID运算
	
	if(M1.PWM > POWER_MAX)  M1.PWM =  POWER_MAX;	 //输出限幅
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX;		 	
	if(M2.PWM > POWER_MAX)  M2.PWM =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX;		

	MotorMove(M1.PWM,M2.PWM);				 //电机动作
}
/*------------------------------------------------------*
//第四问的实现：
//将X、Y轴设定值都为0，然后PID运算跟踪
//在5s内使风摆静止							  //此问不再更改
//此问的PID参数与其它问不同
//------------------------------------------------------*/
void Mode_4(void)
{
	if(abs(M1.CurPos)<40.0 && abs(M2.CurPos)<40.0)	     //X与Y轴角度小于40°才制动
	{		
		PID_M1_SetPoint(0);	                       //M1设定参数
		PID_M1_SetKp(18);				   
		PID_M1_SetKi(0.01);	 			   
		PID_M1_SetKd(2600);				   
		PID_M2_SetPoint(0);		                 //M2设定参数
		PID_M2_SetKp(18);    
		PID_M2_SetKi(0.01);		
		PID_M2_SetKd(2600);
			
		M1.PWM = PID_M1_PosLocCalc(M1.CurPos);         //电机1、3 PID运算
		M2.PWM = PID_M2_PosLocCalc(M2.CurPos);         //电机2、4 PID运算
		
		if(M1.PWM > POWER_MAX)  M1.PWM = 1300;
		if(M1.PWM < -POWER_MAX) M1.PWM = -1300;
	
		if(M2.PWM > POWER_MAX)  M2.PWM = 1300;
		if(M2.PWM < -POWER_MAX) M2.PWM = -1300;        //输出限幅
	}
	else	
	{
	 	M1.PWM = 10L;
		M2.PWM = 10L;		                       //角度过大，停机
	}	
	MotorMove(M1.PWM,M2.PWM);	                       //电机动作
}
/*-----------------------------------------------------*
//第五问的实现：
//半径可设置的圆周运动
//若两个简谐运动的相位差为π/2或π/2，则曲线是直线段		//此问不再更改
// ψ = π/2  ，逆时针转
// ψ = 3π/2 ，顺时针转
//-----------------------------------------------------*/	
void Mode_5(void)
{
	const float priod = 1600.0;                  //单摆周期1600ms 
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
	
	if(RoundDir == 0)  phase = 3.141592/2.0;     //逆时针旋转相位差90° （π/2）	  			         
	else   phase = (3.0*3.141592)/2.0;	         //顺时针旋转相位差270°（π/2）
	
	set_x = A*sin(Omega);			         //计算X轴摆幅分量，换算为弧度
	set_y = A*sin(Omega+phase); 	               //计算Y轴摆幅分量，换算为弧度
	 
	PID_M1_SetPoint(set_y);	                     //M1设定参数
	PID_M1_SetKp(36);				   	   
	PID_M1_SetKi(0.198);	 			   
	PID_M1_SetKd(1100);				   
	PID_M2_SetPoint(set_x);		               //M2设定参数
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 		 
	
	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);       //电机1、3 PID运算
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);       //电机2、4 PID运算
	
	if(M1.PWM > POWER_MAX)  M1.PWM =  POWER_MAX;
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX;
			 	
	if(M2.PWM > POWER_MAX)  M2.PWM =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX; //输出限幅		

	MotorMove(M1.PWM,M2.PWM);			   //电机动作
}
/*------------------------------------------------*
//第六问的实现：
//画圆抗干扰
//包含于第五问中
//------------------------------------------------*/
void Mode_6(void)
{
	const float priod = 1600.0;                  //单摆周期1600ms 
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
	
	if(RoundDir == 0)  phase = 3.141592/2.0;     //逆时针旋转相位差90° （π/2）	  			         
	else   phase = (3.0*3.141592)/2.0;	         //顺时针旋转相位差270°（π/2）
	
	set_x = A*sin(Omega);			         //计算X轴摆幅分量，换算为弧度
	set_y = A*sin(Omega+phase); 	               //计算Y轴摆幅分量，换算为弧度
	 
	PID_M1_SetPoint(set_y);	                     //M1设定参数
	PID_M1_SetKp(36);				   	   
	PID_M1_SetKi(0.198);	 			   
	PID_M1_SetKd(1100);				   
	PID_M2_SetPoint(set_x);		               //M2设定参数
	PID_M2_SetKp(36);    
	PID_M2_SetKi(0.198);		
	PID_M2_SetKd(1100); 		 
	
	M1.PWM = PID_M1_PosLocCalc(M1.CurPos);       //电机1、3 PID运算
	M2.PWM = PID_M2_PosLocCalc(M2.CurPos);       //电机2、4 PID运算
	
	if(M1.PWM > POWER_MAX)  M1.PWM =  POWER_MAX;
	if(M1.PWM < -POWER_MAX) M1.PWM = -POWER_MAX;
			 	
	if(M2.PWM > POWER_MAX)  M2.PWM =  POWER_MAX;
	if(M2.PWM < -POWER_MAX) M2.PWM = -POWER_MAX; //输出限幅		

	MotorMove(M1.PWM,M2.PWM);			   //电机动作
}
//------------------------------------------
//       电机1PID参数结构体初始化
//   这里的电机1代表实际的电机1号和3号		
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
//       电机2PID参数结构体初始化
//  	这里的电机1代表实际的电机2号和4号	
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
//          电机1PID参数设定		
//------------------------------------------
void PID_M1_SetPoint(float setpoint)
{	
	M1PID.SetPoint = setpoint;		//设 定 目标值：M1、M3
}
void PID_M1_SetKp(float dKpp)
{	
	M1PID.Proportion = dKpp;		//设定比例系数：M1、M3
}
void PID_M1_SetKi(float dKii)
{	
	M1PID.Integral = dKii;			//设定积分系数：M1、M3
}
void PID_M1_SetKd(float dKdd)
{	
	M1PID.Derivative = dKdd;		//设定微分系数：M1、M3
}
//------------------------------------------
//	      电机2PID参数设定
//------------------------------------------
void PID_M2_SetPoint(float setpoint)
{	
	M2PID.SetPoint = setpoint;		//设 定 目标值：M2、M4
}
void PID_M2_SetKp(float dKpp)
{	
	M2PID.Proportion = dKpp;		//设定比例系数：M2、M4
}
void PID_M2_SetKi(float dKii)
{	
	M2PID.Integral = dKii;			//设定积分系数：M2、M4
}
void PID_M2_SetKd(float dKdd)
{	
	M2PID.Derivative = dKdd;		//设定微分系数：M2、M4
}
//------------------------------------------
//功能：电机1位置式PID计算
//参数说明：*****
//	入口参数：风摆当前位置（Y轴角度）
//    出口参数：电机1和3的PWM量
//------------------------------------------
int PID_M1_PosLocCalc(float NextPoint)
{
      float  iError,dError;   
	iError = M1PID.SetPoint - NextPoint;        //当前误差 = 设定角度 - 实际角度
	M1PID.SumError += iError;			  //误差累积（积分）
	if(M1PID.SumError > 2300.0)			  //积分限幅
		M1PID.SumError = 2300.0;
	else if(M1PID.SumError < -2300.0)
		M1PID.SumError = -2300.0;	
	dError = iError - M1PID.LastError; 		  //微分
	M1PID.LastError = iError;			  //误差更新
	return(int)  (  M1PID.Proportion   * iError          	//离散PID计算 
          		    + M1PID.Integral   * M1PID.SumError 	 
          		    + M1PID.Derivative * dError);
}
//------------------------------------------
//功能：电机2位置式PID计算
//参数说明：*****
//    入口参数：风摆当前位置（X轴角度）
//    出口参数：电机2和4的PWM量		
//------------------------------------------
int PID_M2_PosLocCalc(float NextPoint)
{
	float  iError,dError;
	iError = M2PID.SetPoint - NextPoint;        //当前误差 = 设定角度 - 实际角度
	M2PID.SumError += iError;			  //误差累积（积分）
	if(M2PID.SumError > 2300.0)			  //积分限幅
		M2PID.SumError = 2300.0;
	else if(M2PID.SumError < -2300.0)
		M2PID.SumError = -2300.0;
	dError = iError - M2PID.LastError; 		  //微分
	M2PID.LastError = iError;			  //误差更新	
	return(int)(  M2PID.Proportion   * iError           	
          		  + M2PID.Integral   * M2PID.SumError 	//离散PID计算 
          		  + M2PID.Derivative * dError);
}
//------------------------------------------
//功能：电机底层驱动函数
//参数说明：
//pwm1：电机1、3共享，控制Y轴运动（Roll）
//pwm2: 电机2、4共享，控制X轴运动（pitch）
//出口参数：无
//------------------------------------------
void MotorMove(long pwm1,long pwm2)
{
	if(pwm1 > 0)			      /*风摆由正角度开始起摆*/
	{						/*向左摆动（角度为正），误差为正，pid运算结果(pwm1)为正*/
	 	SetPWM_M1(1L);		      /*向右摆动（角度为负），误差为负，pid运算结果(pwm1)为负*/
		SetPWM_M3(pwm1);	      	/*pwm为正，表示风摆左摆，电机3动作，电机1停机*/
	}						/*pwm为负，表示风摆右摆，电机1动作，电机3停机*/
	else if(pwm1 < 0)				/*硬件情况：*/
	{						/*风摆向左摆，Y轴角度为正*/
	 	SetPWM_M1(abs(pwm1));		/*风摆向后摆，X轴角度为正*/
		SetPWM_M3(1L);			/*反之*/
	}						/*电机只可动作与停机，无反转功能*/
	if(pwm2 > 0)
	{
	 	SetPWM_M4(1L);		    	//电机4停机
		SetPWM_M2(pwm2);			//电机2动作
	}
	else if(pwm2 < 0)
	{
	 	SetPWM_M4(abs(pwm2));		//电机4动作
		SetPWM_M2(1L);			//电机2停机
	} 	
}