#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************

// void TIM3_Int_Init(u16 arr,u16 psc);
// void TIM3_PWM_Init(u16 arr,u16 psc);
// void TIM2_PWM_Init(u16 arr,u16 psc);

// #define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。
// void Encoder_Init_TIM2(void);
// void Encoder_Init_TIM3(void);
// void Encoder_Init_TIM4(void);
// void Encoder_Init_TIM5(void);
// int Read_Encoder(u8 TIMX);


// void TIM1_PWM_Init(u16 arr,u16 psc);           //四路电机PWM输出
// void TIM8_PWM_Init(u16 arr,u16 psc);           //四路舵机PWM输出

// void TIM4_Mode_Config(void);                   //霍尔编码器AB正交解码
// void TIM3_Mode_Config(void);                   //霍尔编码器AB正交解码
// void TIM5_Mode_Config(void);                   //霍尔编码器AB正交解码
// void TIM2_Mode_Config(void);                   //霍尔编码器AB正交解码

void TIM7_init(u16 arr,u16 psc);               //定时器TIM7，用定时器中断运行周期算法，PID算法




#endif
