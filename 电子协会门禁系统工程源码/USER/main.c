#include "led.h"
#include "beep.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "oled.h"
#include "math.h"
#include "key.h"
#include "spi.h"
#include "rc522_config.h"
#include "rc522_function.h"
#include "RC522.H"
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h" 
#include "timer.h"
#include "Control.h"

/**************************************************
//ƽ̨��STM32F103ZET6��Сϵͳ���İ�
//���ߣ����ԣ
//�ص㣺�㶫��ҵ��ѧ����ѧԺ
//���ܣ�RC522��Ƶ���Ž�ϵͳ
//�ο�������ԭ��@ALIENTEK
//���ڣ�2019/6/1
//ת����Ҫע������
***************************************************/
//ͨ�����ڴ�ӡSD�������Ϣ
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								               //����Ե�ַ
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//��ʾ���С
}  

 int main(void)
 {	

		delay_init();	    	               //��ʱ������ʼ��	  
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		LED_Init();		  	                 //��ʼ����LED���ӵ�Ӳ���ӿ�
		KEY_Init();					               //��ʼ������
		BEEP_Init();                       //��������ʼ��
		TIM7_init(100-1,7200-1);           //10ms��ʱ
		uart1_init(115200);                //UART1��ʼ����������Ϊ115200
	 	usmart_dev.init(72);		           //��ʼ��USMART		
	 	OLED_Init();		                 	//��ʼ��OLED  
		OLED_Clear();	                     //����
	  W25QXX_Init();			             	//��ʼ��W25Q128 ��ΪSPI �ⲿFLASH�������Ž�ϵͳû��ʹ�õ��������ػ�����Ҫ��ʼ������Ϊ��ֲ�ļ�ϵͳ��������Ҫ�õ��ˡ�
	  my_mem_init(SRAMIN);              //�ڴ�����ʼ��
		RC522_Init ();	                  //��ʼ��RC522ģ�飬Ҳ�������ǵĶ�����
		printf ( "����Э���Ž�ϵͳ\r\n" );
	  PcdReset ();                      //��λRC522 
	  M500PcdConfigISOType ( 'A' );     //���ù�����ʽ
	 
 		while(SD_Init())                  //��ⲻ��SD����û�忨ʱ
		{					
 			delay_ms(500);
			LED0=~LED0;//DS0��˸
			OLED_ShowCHinese(10,0,14);//��
			OLED_ShowCHinese(28,0,15);//��
			OLED_ShowCHinese(46,0,16);//��
			OLED_ShowCHinese(64,0,17);//��
			OLED_ShowCHinese(82,0,18);//��
			OLED_ShowCHinese(100,0,19);//��
			OLED_ShowString(0,2,"SD Card Error!");
		}
		show_sdcard_info();	//��ӡSD�������Ϣ
		
		exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
		f_mount(fs[0],"0:",1); 					//����SD�� 

		OLED_Clear();	
		OLED_DrawBMP(0,0,128,8,LOGO_BMP);        //��ʼ����ʾ����Э��LOGO
		delay_ms(5000);
		OLED_Clear();	                          //����
		
		OLED_ShowCHinese(10,0,0);//��            //��ʾ����
		OLED_ShowCHinese(28,0,1);//��
		OLED_ShowCHinese(46,0,2);//Э
		OLED_ShowCHinese(64,0,3);//��
		OLED_ShowCHinese(82,0,4);//��
		OLED_ShowCHinese(100,0,5);//��

		f_open(file,"0:/CARD_UID.csv",FA_WRITE|FA_OPEN_ALWAYS|FA_READ);
 		f_close(file);

		while(1)
		{		
			
 			IC_test();                          //ˢ������⣬���ڴ濨��ƥ�䶼���������
			if(Open_Door_flag==0)            
			{
				OLED_ShowCHinese(10,0,0);//��     //��ʾ����
				OLED_ShowCHinese(28,0,1);//��
				OLED_ShowCHinese(46,0,2);//Э
				OLED_ShowCHinese(64,0,3);//��
				OLED_ShowCHinese(82,0,4);//��
				OLED_ShowCHinese(100,0,5);//��
			}
			if(Open_Door_flag==3)    //�������ڿ���״̬���������ţ��ƣ�OLED�ޱ仯
			{
				Open_Door_flag=0;
				PFout(2)=1;							
				delay_ms(50);
				PFout(2)=0;
			}

			
		}
		
 }

 
