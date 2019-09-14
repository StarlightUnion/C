#ifndef _KEY_H_
#define _KEY_H_
/////////////////////////////////////////////////////
//-------------------------------------------------
extern  uint   CurMode;		 //执行问题
extern  bit    RoundDir;       //圆周运动正反转控制
extern  float  R; 	    	 //摆动半径(cm)
extern  float  angle;	       //摆动角度（第三问）
void Delay1ms(void);           //系统通用延时  @24.000MHz 1ms
void Key_Scan(void);		 //键盘扫描
//-------------------------------------------------
//////////////////////////////////////////////////////
#endif