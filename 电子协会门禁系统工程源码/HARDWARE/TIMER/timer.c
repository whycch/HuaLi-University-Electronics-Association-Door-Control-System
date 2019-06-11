#include "timer.h"
#include "led.h"
#include "usart.h"  
#include "sys.h"


/***************************************
说明：
定时器TIM7初始化
在定时器TIM7中断定时检测Open_Door_flag标志位同时OLED显示不同画面

*************************************/
void TIM7_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //①时钟 TIM7 使能

	//定时器TIM7初始化
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =7200-1; //设置时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);  //②初始化 TIM7
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );  //③允许更新中断
	//中断优先级 NVIC 设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级 3 级

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ 通道被使能
	NVIC_Init(&NVIC_InitStructure);   //④初始化 NVIC 寄存器
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );  // 允许更新中断
	TIM_Cmd(TIM7, ENABLE);  //⑤使能 TIM7

}

/*********************************************************
以下是其他定时器初始化函数，使用时自己删除注释

**************************************************************/


// // /*******************************************************
// // 说明：
// // 电机控制定时器TIM1，用于输出四路PWM控制小车
// // Channal1 PE9
// // Channal2 PE11
// // Channal3 PE13
// // Channal4 PE14

// // *******************************************************/

// // void TIM1_PWM_Init(u16 arr,u16 psc)
// // {
// // 	GPIO_InitTypeDef GPIO_InitStructure;//定义一个结构体变量，内含三个成员
// // 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// // 	TIM_OCInitTypeDef  TIM_OCInitStructure;	

// // 					//------需要修改TIMx
// // 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//使能定时器1时钟	
// // 	
// // 	/*****PWM输出GPIO口初始化**********/
// // 						//------需要修改GPIOx
// //  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能GPIO外设时钟		
// //  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//使能AFIO复用功能模块时钟，不能去掉本条语句	

// // 	
// // 						//------根据需要选择是否映射------需要修改TIMx
// // 	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIMx, ENABLE); 	//Timerx部分重映射
// // 	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); 	//Timerx完全重映射
// // 	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, DISABLE);	//Timer1部分重映射禁止
// // 	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, DISABLE); 		//Timer1完全重映射禁止
// // 									//GPIO_FullRemap_TIMx时表示完全映射 
// // 	
// // 	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;  //
// // 	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  //作为PWM输出时GPIO口必须使用复用推挽输出
// // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// // 						//------需要修改GPIOx
// // 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO
// // 	

// // 	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
// // 	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
// // 	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
// // 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
// // 	
// // 	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

// // 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式设置

// // 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
// // 	
// // 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
// // 	
// // 	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //TIM1通道设定
// // 	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
// // 	
// // 	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //TIM1通道设定
// // 	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
// // 	
// // 	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //TIM1通道设定
// // 	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR3上的预装载寄存器
// // 	
// // 	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //TIM1通道设定
// // 	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR4上的预装载寄存器
// // 	
// // 	TIM_Cmd(TIM1, ENABLE);  //使能TIM1	
// // 	TIM_CtrlPWMOutputs(TIM1, ENABLE);                                   //pwm输出使能，一定要记得打开
// // 	
// // }

// // /***************************************
// // 说明：
// // 电机控制定时器TIM8，用于输出四路PWM控制舵机
// // Channal1 PC6
// // Channal2 PC7
// // Channal3 PC8
// // Channal4 PC9

// // ***************************************/

// // void TIM8_PWM_Init(u16 arr,u16 psc)
// // {
// // 	GPIO_InitTypeDef GPIO_InitStructure;
// // 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// // 	TIM_OCInitTypeDef  TIM_OCInitStructure;
// // 	
// // 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	//使能定时器8时钟
// // 	
// // 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟 
// // 	
// // 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
// // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
// // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// // 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
// // 	
// // 	 //初始化TIM8
// // 	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
// // 	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
// // 	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
// // 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
// // 	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
// // 	
// // 	//初始化TIM8 Channel234 PWM模式	 
// // 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
// //  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
// // 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
// // 	
// // 	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
// // 	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
// // 	
// // 	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
// // 	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
// // 	
// // 	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
// // 	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
// // 	
// // 	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2
// // 	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
// // 	
// // 	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
// // 	TIM_CtrlPWMOutputs(TIM8, ENABLE);                             //pwm输出
// // 	
// // }
// // /***********************************************
// // 霍尔编码器
// // 通过定时器做编码器正交解码（捕获），必须是
// // 用定时器的Channal1,Channal2,通道1和通道2。
// // 高级定时器TIM1、TIM8和普通定时器TIM2、TIM3
// // TIM4、TIM5，都可以做编码器脉冲捕获，但是基
// // 本定时器不行
// // ***********************************************/


// // /***************************************
// // 说明：
// // 定时器TIM4，做霍尔编码器的AB相正交解码
// // A相------PB6
// // B相------PB7
// // *************************************/
// // void TIM4_Mode_Config(void)
// // {
// //   		GPIO_InitTypeDef         GPIO_InitStructure;
// // 		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
// // 		TIM_ICInitTypeDef TIM_ICInitStructure; 
// //   
// // 	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	 //使能TIM4时钟
// //  	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
// // 	
// // 	
// // 	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7 ;            
// // 	  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	         
// // 	  GPIO_Init(GPIOB, &GPIO_InitStructure);
// // 	  GPIO_ResetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);
// //   

// //   /* Timer configuration in Encoder mode */					//设置ENCODER_TIMER为编码器模式
// //     TIM_DeInit(TIM4);							   //ENCODER_TIMER复位
// // 		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// // 		TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
// // 		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //设定计数器自动重装值
// // 		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
// // 		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
// // 		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
// // 		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//编码器接口初始化 TIM_ICPolarity_Rising上升沿捕获
// // 		TIM_ICStructInit(&TIM_ICInitStructure);
// // 		TIM_ICInitStructure.TIM_ICFilter = 10;
// // 		TIM_ICInit(TIM4, &TIM_ICInitStructure);
// // 		TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
// // 		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
// // 		//Reset counter
// // 		TIM_SetCounter(TIM4,0);
// // 		TIM_Cmd(TIM4, ENABLE); 
// // }
// // /***************************************
// // 说明：
// // 定时器TIM3，做霍尔编码器的AB相正交解码
// // A相------PA6
// // B相------PA7
// // *************************************/
// // void TIM3_Mode_Config(void)
// // {
// //   	GPIO_InitTypeDef         GPIO_InitStructure;
// // 		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
// // 		TIM_ICInitTypeDef TIM_ICInitStructure; 
// //   
// // 	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	 //使能TIM3时钟
// //  	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
// // 	
// // 	
// // 	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7 ;            
// // 	  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	         
// // 	  GPIO_Init(GPIOA, &GPIO_InitStructure);
// // 	  GPIO_ResetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);
// //   

// //   /* Timer configuration in Encoder mode */					//设置ENCODER_TIMER为编码器模式
// //     TIM_DeInit(TIM3);							   //ENCODER_TIMER复位
// // 		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// // 		TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
// // 		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //设定计数器自动重装值
// // 		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
// // 		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
// // 		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
// // 		TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//编码器接口初始化 TIM_ICPolarity_Rising上升沿捕获
// // 		TIM_ICStructInit(&TIM_ICInitStructure);
// // 		TIM_ICInitStructure.TIM_ICFilter = 10;
// // 		TIM_ICInit(TIM3, &TIM_ICInitStructure);
// // 		TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM的更新标志位
// // 		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
// // 		//Reset counter
// // 		TIM_SetCounter(TIM3,0);
// // 		TIM_Cmd(TIM3, ENABLE); 
// // }
// // /***************************************
// // 说明：
// // 定时器TIM5，做霍尔编码器的AB相正交解码
// // A相------PA0
// // B相------PA1
// // *************************************/
// // void TIM5_Mode_Config(void)
// // {
// //   	GPIO_InitTypeDef         GPIO_InitStructure;
// // 		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
// // 		TIM_ICInitTypeDef TIM_ICInitStructure; 
// //   
// // 	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	 //使能TIM5时钟
// //  	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
// // 	
// // 	
// // 	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 ;            
// // 	  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	         
// // 	  GPIO_Init(GPIOA, &GPIO_InitStructure);
// // 	  GPIO_ResetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1);
// //   

// //   /* Timer configuration in Encoder mode */					//设置ENCODER_TIMER为编码器模式
// //     TIM_DeInit(TIM5);							   //ENCODER_TIMER复位
// // 		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// // 		TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
// // 		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //设定计数器自动重装值
// // 		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
// // 		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
// // 		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
// // 		TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//编码器接口初始化 TIM_ICPolarity_Rising上升沿捕获
// // 		TIM_ICStructInit(&TIM_ICInitStructure);
// // 		TIM_ICInitStructure.TIM_ICFilter = 10;
// // 		TIM_ICInit(TIM5, &TIM_ICInitStructure);
// // 		TIM_ClearFlag(TIM5, TIM_FLAG_Update);//清除TIM的更新标志位
// // 		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
// // 		//Reset counter
// // 		TIM_SetCounter(TIM5,0);
// // 		TIM_Cmd(TIM5, ENABLE); 
// // }
// // /***************************************
// // 说明：
// // 定时器TIM2，做霍尔编码器的AB相正交解码
// // A相------PA15
// // B相------PB3
// // *************************************/
// // void TIM2_Mode_Config(void)
// // {
// // 	  GPIO_InitTypeDef GPIO_InitStructure;
// //     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// //     TIM_ICInitTypeDef TIM_ICInitStructure;      

// //     //PA15 ch1, PB3 ch2 
// //     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
// //     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

// //     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);        // 关闭JTAG功能 ,如果是J-Link是JTAG下载方式，需要在Debbug里面修改为SW下载方式
// //     GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);            // 重映射TIM2的CH1、CH2到PA15和PB3
// //     GPIO_StructInit(&GPIO_InitStructure);
// //     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;         
// //     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// //     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// //     GPIO_Init(GPIOA, &GPIO_InitStructure);                           
// //     GPIO_StructInit(&GPIO_InitStructure);
// //     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;         
// //     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// //     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// //     GPIO_Init(GPIOB, &GPIO_InitStructure);  
// //   /* Timer configuration in Encoder mode */					//设置ENCODER_TIMER为编码器模式
// //     TIM_DeInit(TIM2);							   //ENCODER_TIMER复位
// // 		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// // 		TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
// // 		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //设定计数器自动重装值
// // 		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
// // 		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
// // 		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
// // 		TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//编码器接口初始化 TIM_ICPolarity_Rising上升沿捕获
// // 		TIM_ICStructInit(&TIM_ICInitStructure);
// // 		TIM_ICInitStructure.TIM_ICFilter = 10;
// // 		TIM_ICInit(TIM2, &TIM_ICInitStructure);
// // 		TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
// // 		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
// // 		//Reset counter
// // 		TIM_SetCounter(TIM2,0);
// // 		TIM_Cmd(TIM2, ENABLE); 

// // }


