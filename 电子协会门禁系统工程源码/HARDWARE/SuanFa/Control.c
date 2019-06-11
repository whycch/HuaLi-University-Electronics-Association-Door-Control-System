#include "Control.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "beep.h"
#include "RC522.H"
#include "oled.h"
#include "led.h"

/***************************************************************************
//ƽ̨��STM32F103ZET6��Сϵͳ���İ�
//���ߣ����ԣ
//�ص㣺�㶫��ҵ��ѧ����ѧԺ
//�ο�������ԭ��@ALIENTEK
//���ڣ�2019/6/1
//���ܣ��ڶ�ʱ���ж���ˢ����ʾOLED����
***************************************************************************/


unsigned int time=0;
u16 UID_Show_Time_flag=0;

void TIM7_IRQHandler(void) //TIM7 10ms�ж�
{ 
 static u8 clean_flag=0;
 static u8 state=0;
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) //��� TIM7 �����жϷ������
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update ); //��� TIM7 �����жϱ�־
		time++;
		if((time>=50)&&(Open_Door_flag==0))       //��û��ˢ��״̬�£�WS2812RGB�ƺ�ɫ��LED2���500ms��˸
		{
			time=0;
			LED2=~LED2;
			if(state==0)
			{
				state=1;
				ws281x_sendOne(0x000000);
			}
			else if(state==1)
			{
					state=0;
					ws281x_sendOne(0x55ff66);
			}
			
			
		}	
		if(Open_Door_flag==1)              //ˢ���ɹ�״̬��������ƥ��ɹ�
		{
			UID_Show_Time_flag++;
			ws281x_sendOne(0xff0000);        //WS2812RGB�� ��ʾ��ɫ


			if(UID_Show_Time_flag==50)       //OLED��Ļ��ʾ��������UID�Ͱ��շ��������ʽת�����µĿ���500ms��֮���OLED����
			{
				if(clean_flag==0)
				{
					OLED_Clear();
					clean_flag=1;
				}	
				OLED_DrawBMP(0,0,128,8,LOGO_BMP);        //��ʾ����Э��LOGO		
			}	
			else if(UID_Show_Time_flag==100)  //OLED��Ļ��ʾ����Э��LOGO 500ms��֮���OLED����
			{
				UID_Show_Time_flag=0;
				if(clean_flag==1)
				{
					clean_flag=0;
					Open_Door_flag=0;
					LED1=1;
					OLED_Clear();
				}
			}
		}
		else if(Open_Door_flag==2)      //ˢ��ʧ��״̬��������ƥ��ʧ��
		{
			UID_Show_Time_flag++;
			ws281x_sendOne(0x0000ff);     //WS2812RGB�� ��ʾ��ɫ
			
			if(UID_Show_Time_flag%10==0)   //ˢ��ʧ�ܣ�������100ms��� ��~��~��~��~��~
			{
					BEEP=~BEEP;
			}
			
			if(UID_Show_Time_flag==50)
			{
				
				if(clean_flag==0)
				{
					OLED_Clear();
					clean_flag=1;
				}	

			}	
			else if(UID_Show_Time_flag==100)
			{
				UID_Show_Time_flag=0;
				if(clean_flag==1)
				{
					clean_flag=0;
					Open_Door_flag=0;
					OLED_Clear();
				}
			}
		}
		
		
	}
}





