#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************

// void TIM3_Int_Init(u16 arr,u16 psc);
// void TIM3_PWM_Init(u16 arr,u16 psc);
// void TIM2_PWM_Init(u16 arr,u16 psc);

// #define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�
// void Encoder_Init_TIM2(void);
// void Encoder_Init_TIM3(void);
// void Encoder_Init_TIM4(void);
// void Encoder_Init_TIM5(void);
// int Read_Encoder(u8 TIMX);


// void TIM1_PWM_Init(u16 arr,u16 psc);           //��·���PWM���
// void TIM8_PWM_Init(u16 arr,u16 psc);           //��·���PWM���

// void TIM4_Mode_Config(void);                   //����������AB��������
// void TIM3_Mode_Config(void);                   //����������AB��������
// void TIM5_Mode_Config(void);                   //����������AB��������
// void TIM2_Mode_Config(void);                   //����������AB��������

void TIM7_init(u16 arr,u16 psc);               //��ʱ��TIM7���ö�ʱ���ж����������㷨��PID�㷨




#endif
