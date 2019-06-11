#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//LED��������
//ƽ̨��STM32F103ZET6��Сϵͳ���İ�
//���ߣ����ԣ
//�ص㣺�㶫��ҵ��ѧ����ѧԺ
//�ο�������ԭ��@ALIENTEK
//���ڣ�2018/11/4	   
//δ�������ֹת�أ�������������						  
////////////////////////////////////////////////////////////////////////////////// 	
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	
#define LED2 PEout(6)// PE6	

void LED_Init(void);//��ʼ��

void ws281x_delay(unsigned int delay_num);
void ws281x_sendLow(void);   //����0��
void ws281x_sendHigh(void);   //����1��
void ws2811_Reset(void);        //����RESET��		 				    
void ws281x_sendOne(uint32_t dat);  //�������ݿ���һյRGB��  eg: dat=0xff0000;  ��ɫ



#endif




