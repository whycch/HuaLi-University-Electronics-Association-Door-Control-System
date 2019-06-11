#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//LED驱动代码
//平台：STM32F103ZET6最小系统核心板
//作者：吴宏裕
//地点：广东工业大学华立学院
//参考：正点原子@ALIENTEK
//日期：2018/11/4	   
//未经允许禁止转载，不等用于商用						  
////////////////////////////////////////////////////////////////////////////////// 	
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	
#define LED2 PEout(6)// PE6	

void LED_Init(void);//初始化

void ws281x_delay(unsigned int delay_num);
void ws281x_sendLow(void);   //发送0码
void ws281x_sendHigh(void);   //发送1码
void ws2811_Reset(void);        //发送RESET码		 				    
void ws281x_sendOne(uint32_t dat);  //发送数据控制一盏RGB灯  eg: dat=0xff0000;  绿色



#endif




