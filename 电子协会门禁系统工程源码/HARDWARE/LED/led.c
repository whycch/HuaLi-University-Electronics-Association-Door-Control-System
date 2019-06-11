#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//LED��������
//ƽ̨��STM32F103ZET6��Сϵͳ���İ�
//���ߣ����ԣ
//�ص㣺�㶫��ҵ��ѧ����ѧԺ
//�ο�������ԭ��@ALIENTEK
//���ڣ�2018/11/4	   
//δ�������ֹת�أ�������������						  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PB,PE��PF�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;				 //LED1-->PE.5  LED2-->PE.5�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOE
 GPIO_SetBits(GPIOE,GPIO_Pin_5|GPIO_Pin_6);						 //PE5 PE6�����
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //PF0 PF1 PF2 PF3 �˿�����    PF0Ϊ�̵������ƶ�    PF1ΪWS2812RGB�Ƶ�DIN����
 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOF
 GPIO_SetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_3);						 //PF1 PF2 PF3 �����
 GPIO_ResetBits(GPIOF,GPIO_Pin_2);	         //PF0 ���Ϊ�㡢�͵�ƽ����Ϊ���õļ̵����Ǹߵ�ƽ����������Ϊ�͵�ƽ��Ҫ��Ϊ�˳�ʼ��ʱ�̵���Ϊδ����״̬
}



  
/***********************ws2812RGB��ģ��ʹ��************************************************/


/**
  * @brief  ws2812RGB��ģ���õ�����ʱ����
  * @param  delay_num :��ʱ�� ��ʾ����������ʱʱ�� = delay_num * 440ns ��
  * @retval None
  */
void ws281x_delay(unsigned int delay_num)
{
  while(delay_num--);   
}
/**
  * @brief  ����WS281xоƬʱ��ͼ��д�ķ���0�룬1��RRESET��ĺ���
  * @param  
  * @retval None
  */

void ws281x_sendLow(void)   //����0��
{
  PFout(4) = 1;
  ws281x_delay(1);    //ʾ��������ԼΪ440ns
  PFout(4)= 0;
  ws281x_delay(2);
}
void ws281x_sendHigh(void)   //����1��
{
  PFout(4) = 1;
  ws281x_delay(2);
  PFout(4) = 0;
  ws281x_delay(1);
}
void ws2811_Reset(void)        //����RESET��
{ 
  PFout(4)= 0; 
  delay_us(60);  
  PFout(4) = 1; 
  PFout(4) = 0;
}

/**
  * @brief  ���͵���һ���Ƶ����ݣ���24bit��
  * @param  dat����ɫ��24λ����  eg: 0xffffff
  * @retval None
  */
void ws281x_sendOne(uint32_t dat)   
{
  uint8_t i;
  unsigned char byte;
  for(i = 24; i > 0; i--)
  {
    byte = ((dat>>i) & 0x01);  //λ��������ȡdat���ݵĵ�iλ
    if(byte == 1)
    {
      ws281x_sendHigh();
    }
    else
    {
      ws281x_sendLow();
    }
  }
}
/***************************************************************************/

 
