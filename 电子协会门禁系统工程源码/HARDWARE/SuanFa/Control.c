#include "Control.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "beep.h"
#include "RC522.H"
#include "oled.h"
#include "led.h"

/***************************************************************************
//平台：STM32F103ZET6最小系统核心板
//作者：吴宏裕
//地点：广东工业大学华立学院
//参考：正点原子@ALIENTEK
//日期：2019/6/1
//功能：在定时器中断中刷新显示OLED内容
***************************************************************************/


unsigned int time=0;
u16 UID_Show_Time_flag=0;

void TIM7_IRQHandler(void) //TIM7 10ms中断
{ 
 static u8 clean_flag=0;
 static u8 state=0;
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) //检查 TIM7 更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update ); //清除 TIM7 更新中断标志
		time++;
		if((time>=50)&&(Open_Door_flag==0))       //在没有刷卡状态下，WS2812RGB灯红色和LED2间隔500ms闪烁
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
		if(Open_Door_flag==1)              //刷卡成功状态，即卡号匹配成功
		{
			UID_Show_Time_flag++;
			ws281x_sendOne(0xff0000);        //WS2812RGB灯 显示绿色


			if(UID_Show_Time_flag==50)       //OLED屏幕显示读到卡的UID和按照饭卡编码格式转化后新的卡号500ms，之后就OLED清屏
			{
				if(clean_flag==0)
				{
					OLED_Clear();
					clean_flag=1;
				}	
				OLED_DrawBMP(0,0,128,8,LOGO_BMP);        //显示电子协会LOGO		
			}	
			else if(UID_Show_Time_flag==100)  //OLED屏幕显示电子协会LOGO 500ms，之后就OLED清屏
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
		else if(Open_Door_flag==2)      //刷卡失败状态，即卡号匹配失败
		{
			UID_Show_Time_flag++;
			ws281x_sendOne(0x0000ff);     //WS2812RGB灯 显示蓝色
			
			if(UID_Show_Time_flag%10==0)   //刷卡失败，蜂鸣器100ms间隔 哔~哔~哔~哔~哔~
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





