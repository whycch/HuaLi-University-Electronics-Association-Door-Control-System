#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//LED驱动代码
//平台：STM32F103ZET6最小系统核心板
//作者：吴宏裕
//地点：广东工业大学华立学院
//参考：正点原子@ALIENTEK
//日期：2018/11/4	   
//未经允许禁止转载，不等用于商用						  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);	 //使能PB,PE，PF端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;				 //LED1-->PE.5  LED2-->PE.5端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE
 GPIO_SetBits(GPIOE,GPIO_Pin_5|GPIO_Pin_6);						 //PE5 PE6输出高
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //PF0 PF1 PF2 PF3 端口配置    PF0为继电器控制端    PF1为WS2812RGB灯的DIN引脚
 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化GPIOF
 GPIO_SetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_3);						 //PF1 PF2 PF3 输出高
 GPIO_ResetBits(GPIOF,GPIO_Pin_2);	         //PF0 输出为零、低电平，因为我用的继电器是高电平出发，设置为低电平主要是为了初始化时继电器为未触发状态
}



  
/***********************ws2812RGB灯模块使用************************************************/


/**
  * @brief  ws2812RGB灯模块用到的延时函数
  * @param  delay_num :延时数 （示波器测量延时时间 = delay_num * 440ns ）
  * @retval None
  */
void ws281x_delay(unsigned int delay_num)
{
  while(delay_num--);   
}
/**
  * @brief  根据WS281x芯片时序图编写的发送0码，1码RESET码的函数
  * @param  
  * @retval None
  */

void ws281x_sendLow(void)   //发送0码
{
  PFout(4) = 1;
  ws281x_delay(1);    //示波器测试约为440ns
  PFout(4)= 0;
  ws281x_delay(2);
}
void ws281x_sendHigh(void)   //发送1码
{
  PFout(4) = 1;
  ws281x_delay(2);
  PFout(4) = 0;
  ws281x_delay(1);
}
void ws2811_Reset(void)        //发送RESET码
{ 
  PFout(4)= 0; 
  delay_us(60);  
  PFout(4) = 1; 
  PFout(4) = 0;
}

/**
  * @brief  发送点亮一个灯的数据（即24bit）
  * @param  dat：颜色的24位编码  eg: 0xffffff
  * @retval None
  */
void ws281x_sendOne(uint32_t dat)   
{
  uint8_t i;
  unsigned char byte;
  for(i = 24; i > 0; i--)
  {
    byte = ((dat>>i) & 0x01);  //位操作，读取dat数据的第i位
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

 
