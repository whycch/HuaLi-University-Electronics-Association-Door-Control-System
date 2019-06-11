#include "timer.h"
#include "led.h"
#include "usart.h"  
#include "sys.h"


/***************************************
˵����
��ʱ��TIM7��ʼ��
�ڶ�ʱ��TIM7�ж϶�ʱ���Open_Door_flag��־λͬʱOLED��ʾ��ͬ����

*************************************/
void TIM7_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //��ʱ�� TIM7 ʹ��

	//��ʱ��TIM7��ʼ��
	
	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =7200-1; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);  //�ڳ�ʼ�� TIM7
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );  //����������ж�
	//�ж����ȼ� NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ����0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ� 3 ��

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);   //�ܳ�ʼ�� NVIC �Ĵ���
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );  // ��������ж�
	TIM_Cmd(TIM7, ENABLE);  //��ʹ�� TIM7

}

/*********************************************************
������������ʱ����ʼ��������ʹ��ʱ�Լ�ɾ��ע��

**************************************************************/


// // /*******************************************************
// // ˵����
// // ������ƶ�ʱ��TIM1�����������·PWM����С��
// // Channal1 PE9
// // Channal2 PE11
// // Channal3 PE13
// // Channal4 PE14

// // *******************************************************/

// // void TIM1_PWM_Init(u16 arr,u16 psc)
// // {
// // 	GPIO_InitTypeDef GPIO_InitStructure;//����һ���ṹ��������ں�������Ա
// // 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// // 	TIM_OCInitTypeDef  TIM_OCInitStructure;	

// // 					//------��Ҫ�޸�TIMx
// // 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��	
// // 	
// // 	/*****PWM���GPIO�ڳ�ʼ��**********/
// // 						//------��Ҫ�޸�GPIOx
// //  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//ʹ��GPIO����ʱ��		
// //  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//ʹ��AFIO���ù���ģ��ʱ�ӣ�����ȥ���������	

// // 	
// // 						//------������Ҫѡ���Ƿ�ӳ��------��Ҫ�޸�TIMx
// // 	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIMx, ENABLE); 	//Timerx������ӳ��
// // 	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); 	//Timerx��ȫ��ӳ��
// // 	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, DISABLE);	//Timer1������ӳ���ֹ
// // 	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, DISABLE); 		//Timer1��ȫ��ӳ���ֹ
// // 									//GPIO_FullRemap_TIMxʱ��ʾ��ȫӳ�� 
// // 	
// // 	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;  //
// // 	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;  //��ΪPWM���ʱGPIO�ڱ���ʹ�ø����������
// // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// // 						//------��Ҫ�޸�GPIOx
// // 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
// // 	

// // 	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
// // 	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
// // 	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
// // 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
// // 	
// // 	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

// // 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ����

// // 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
// // 	
// // 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
// // 	
// // 	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //TIM1ͨ���趨
// // 	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
// // 	
// // 	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //TIM1ͨ���趨
// // 	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
// // 	
// // 	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //TIM1ͨ���趨
// // 	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR3�ϵ�Ԥװ�ؼĴ���
// // 	
// // 	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //TIM1ͨ���趨
// // 	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR4�ϵ�Ԥװ�ؼĴ���
// // 	
// // 	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1	
// // 	TIM_CtrlPWMOutputs(TIM1, ENABLE);                                   //pwm���ʹ�ܣ�һ��Ҫ�ǵô�
// // 	
// // }

// // /***************************************
// // ˵����
// // ������ƶ�ʱ��TIM8�����������·PWM���ƶ��
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
// // 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	//ʹ�ܶ�ʱ��8ʱ��
// // 	
// // 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ�� 
// // 	
// // 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
// // 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
// // 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// // 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
// // 	
// // 	 //��ʼ��TIM8
// // 	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
// // 	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
// // 	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
// // 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
// // 	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// // 	
// // 	//��ʼ��TIM8 Channel234 PWMģʽ	 
// // 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
// //  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
// // 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
// // 	
// // 	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM8 OC2
// // 	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR2�ϵ�Ԥװ�ؼĴ���
// // 	
// // 	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM8 OC2
// // 	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR2�ϵ�Ԥװ�ؼĴ���
// // 	
// // 	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM8 OC2
// // 	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR2�ϵ�Ԥװ�ؼĴ���
// // 	
// // 	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM8 OC2
// // 	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR2�ϵ�Ԥװ�ؼĴ���
// // 	
// // 	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8
// // 	TIM_CtrlPWMOutputs(TIM8, ENABLE);                             //pwm���
// // 	
// // }
// // /***********************************************
// // ����������
// // ͨ����ʱ�����������������루���񣩣�������
// // �ö�ʱ����Channal1,Channal2,ͨ��1��ͨ��2��
// // �߼���ʱ��TIM1��TIM8����ͨ��ʱ��TIM2��TIM3
// // TIM4��TIM5�������������������岶�񣬵��ǻ�
// // ����ʱ������
// // ***********************************************/


// // /***************************************
// // ˵����
// // ��ʱ��TIM4����������������AB����������
// // A��------PB6
// // B��------PB7
// // *************************************/
// // void TIM4_Mode_Config(void)
// // {
// //   		GPIO_InitTypeDef         GPIO_InitStructure;
// // 		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
// // 		TIM_ICInitTypeDef TIM_ICInitStructure; 
// //   
// // 	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	 //ʹ��TIM4ʱ��
// //  	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
// // 	
// // 	
// // 	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7 ;            
// // 	  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	         
// // 	  GPIO_Init(GPIOB, &GPIO_InitStructure);
// // 	  GPIO_ResetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);
// //   

// //   /* Timer configuration in Encoder mode */					//����ENCODER_TIMERΪ������ģʽ
// //     TIM_DeInit(TIM4);							   //ENCODER_TIMER��λ
// // 		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// // 		TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
// // 		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //�趨�������Զ���װֵ
// // 		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
// // 		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
// // 		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
// // 		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//�������ӿڳ�ʼ�� TIM_ICPolarity_Rising�����ز���
// // 		TIM_ICStructInit(&TIM_ICInitStructure);
// // 		TIM_ICInitStructure.TIM_ICFilter = 10;
// // 		TIM_ICInit(TIM4, &TIM_ICInitStructure);
// // 		TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
// // 		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
// // 		//Reset counter
// // 		TIM_SetCounter(TIM4,0);
// // 		TIM_Cmd(TIM4, ENABLE); 
// // }
// // /***************************************
// // ˵����
// // ��ʱ��TIM3����������������AB����������
// // A��------PA6
// // B��------PA7
// // *************************************/
// // void TIM3_Mode_Config(void)
// // {
// //   	GPIO_InitTypeDef         GPIO_InitStructure;
// // 		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
// // 		TIM_ICInitTypeDef TIM_ICInitStructure; 
// //   
// // 	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	 //ʹ��TIM3ʱ��
// //  	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
// // 	
// // 	
// // 	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7 ;            
// // 	  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	         
// // 	  GPIO_Init(GPIOA, &GPIO_InitStructure);
// // 	  GPIO_ResetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);
// //   

// //   /* Timer configuration in Encoder mode */					//����ENCODER_TIMERΪ������ģʽ
// //     TIM_DeInit(TIM3);							   //ENCODER_TIMER��λ
// // 		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// // 		TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
// // 		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //�趨�������Զ���װֵ
// // 		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
// // 		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
// // 		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
// // 		TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//�������ӿڳ�ʼ�� TIM_ICPolarity_Rising�����ز���
// // 		TIM_ICStructInit(&TIM_ICInitStructure);
// // 		TIM_ICInitStructure.TIM_ICFilter = 10;
// // 		TIM_ICInit(TIM3, &TIM_ICInitStructure);
// // 		TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
// // 		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
// // 		//Reset counter
// // 		TIM_SetCounter(TIM3,0);
// // 		TIM_Cmd(TIM3, ENABLE); 
// // }
// // /***************************************
// // ˵����
// // ��ʱ��TIM5����������������AB����������
// // A��------PA0
// // B��------PA1
// // *************************************/
// // void TIM5_Mode_Config(void)
// // {
// //   	GPIO_InitTypeDef         GPIO_InitStructure;
// // 		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
// // 		TIM_ICInitTypeDef TIM_ICInitStructure; 
// //   
// // 	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	 //ʹ��TIM5ʱ��
// //  	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
// // 	
// // 	
// // 	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 ;            
// // 	  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	         
// // 	  GPIO_Init(GPIOA, &GPIO_InitStructure);
// // 	  GPIO_ResetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1);
// //   

// //   /* Timer configuration in Encoder mode */					//����ENCODER_TIMERΪ������ģʽ
// //     TIM_DeInit(TIM5);							   //ENCODER_TIMER��λ
// // 		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// // 		TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
// // 		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //�趨�������Զ���װֵ
// // 		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
// // 		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
// // 		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
// // 		TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//�������ӿڳ�ʼ�� TIM_ICPolarity_Rising�����ز���
// // 		TIM_ICStructInit(&TIM_ICInitStructure);
// // 		TIM_ICInitStructure.TIM_ICFilter = 10;
// // 		TIM_ICInit(TIM5, &TIM_ICInitStructure);
// // 		TIM_ClearFlag(TIM5, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
// // 		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
// // 		//Reset counter
// // 		TIM_SetCounter(TIM5,0);
// // 		TIM_Cmd(TIM5, ENABLE); 
// // }
// // /***************************************
// // ˵����
// // ��ʱ��TIM2����������������AB����������
// // A��------PA15
// // B��------PB3
// // *************************************/
// // void TIM2_Mode_Config(void)
// // {
// // 	  GPIO_InitTypeDef GPIO_InitStructure;
// //     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// //     TIM_ICInitTypeDef TIM_ICInitStructure;      

// //     //PA15 ch1, PB3 ch2 
// //     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
// //     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

// //     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);        // �ر�JTAG���� ,�����J-Link��JTAG���ط�ʽ����Ҫ��Debbug�����޸�ΪSW���ط�ʽ
// //     GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);            // ��ӳ��TIM2��CH1��CH2��PA15��PB3
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
// //   /* Timer configuration in Encoder mode */					//����ENCODER_TIMERΪ������ģʽ
// //     TIM_DeInit(TIM2);							   //ENCODER_TIMER��λ
// // 		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
// // 		TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
// // 		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //�趨�������Զ���װֵ
// // 		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
// // 		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
// // 		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
// // 		TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//�������ӿڳ�ʼ�� TIM_ICPolarity_Rising�����ز���
// // 		TIM_ICStructInit(&TIM_ICInitStructure);
// // 		TIM_ICInitStructure.TIM_ICFilter = 10;
// // 		TIM_ICInit(TIM2, &TIM_ICInitStructure);
// // 		TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
// // 		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
// // 		//Reset counter
// // 		TIM_SetCounter(TIM2,0);
// // 		TIM_Cmd(TIM2, ENABLE); 

// // }


