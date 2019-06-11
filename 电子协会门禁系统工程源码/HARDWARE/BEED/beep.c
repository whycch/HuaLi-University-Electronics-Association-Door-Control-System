#include "beep.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//蜂鸣器驱动代码
//平台：STM32F103ZET6最小系统核心板
//作者：吴宏裕
//地点：广东工业大学华立学院
//参考：正点原子@ALIENTEK
//日期：2018/11/4	   
//未经允许禁止转载，不等用于商用						  
////////////////////////////////////////////////////////////////////////////////// 	
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);			//使能GPIOD端口定时器
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                 //PD11端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速度为50MHz
	
	GPIO_Init(GPIOD,&GPIO_InitStructure);                     //根据参数初始化GPIOD.11
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);                         //输出0，关闭蜂鸣器
	
}

void Beed()
{
	GPIO_SetBits(GPIOD,GPIO_Pin_11);                         //输出1,开蜂鸣器
	delay_ms(200);
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);                         //输出0，关闭蜂鸣器
	delay_ms(30);
	
}
