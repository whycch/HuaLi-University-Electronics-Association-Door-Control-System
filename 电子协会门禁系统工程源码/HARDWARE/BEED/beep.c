#include "beep.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//��������������
//ƽ̨��STM32F103ZET6��Сϵͳ���İ�
//���ߣ����ԣ
//�ص㣺�㶫��ҵ��ѧ����ѧԺ
//�ο�������ԭ��@ALIENTEK
//���ڣ�2018/11/4	   
//δ�������ֹת�أ�������������						  
////////////////////////////////////////////////////////////////////////////////// 	
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);			//ʹ��GPIOD�˿ڶ�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                 //PD11�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //�ٶ�Ϊ50MHz
	
	GPIO_Init(GPIOD,&GPIO_InitStructure);                     //���ݲ�����ʼ��GPIOD.11
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);                         //���0���رշ�����
	
}

void Beed()
{
	GPIO_SetBits(GPIOD,GPIO_Pin_11);                         //���1,��������
	delay_ms(200);
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);                         //���0���رշ�����
	delay_ms(30);
	
}
