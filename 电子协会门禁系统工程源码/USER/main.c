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
//平台：STM32F103ZET6最小系统核心板
//作者：吴宏裕
//地点：广东工业大学华立学院
//功能：RC522射频卡门禁系统
//参考：正点原子@ALIENTEK
//日期：2019/6/1
//转载需要注明出处
***************************************************/
//通过串口打印SD卡相关信息
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//制造商ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								               //卡相对地址
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//显示块大小
}  

 int main(void)
 {	

		delay_init();	    	               //延时函数初始化	  
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
		LED_Init();		  	                 //初始化与LED连接的硬件接口
		KEY_Init();					               //初始化按键
		BEEP_Init();                       //蜂鸣器初始化
		TIM7_init(100-1,7200-1);           //10ms定时
		uart1_init(115200);                //UART1初始化，波特率为115200
	 	usmart_dev.init(72);		           //初始化USMART		
	 	OLED_Init();		                 	//初始化OLED  
		OLED_Clear();	                     //清屏
	  W25QXX_Init();			             	//初始化W25Q128 此为SPI 外部FLASH，但是门禁系统没有使用到。不过呢还是需要初始化，因为移植文件系统过程中需要用到了。
	  my_mem_init(SRAMIN);              //内存管理初始化
		RC522_Init ();	                  //初始化RC522模块，也就是我们的读卡器
		printf ( "电子协会门禁系统\r\n" );
	  PcdReset ();                      //复位RC522 
	  M500PcdConfigISOType ( 'A' );     //设置工作方式
	 
 		while(SD_Init())                  //检测不到SD卡，没插卡时
		{					
 			delay_ms(500);
			LED0=~LED0;//DS0闪烁
			OLED_ShowCHinese(10,0,14);//请
			OLED_ShowCHinese(28,0,15);//插
			OLED_ShowCHinese(46,0,16);//入
			OLED_ShowCHinese(64,0,17);//内
			OLED_ShowCHinese(82,0,18);//存
			OLED_ShowCHinese(100,0,19);//卡
			OLED_ShowString(0,2,"SD Card Error!");
		}
		show_sdcard_info();	//打印SD卡相关信息
		
		exfuns_init();							//为fatfs相关变量申请内存				 
		f_mount(fs[0],"0:",1); 					//挂载SD卡 

		OLED_Clear();	
		OLED_DrawBMP(0,0,128,8,LOGO_BMP);        //初始化显示电子协会LOGO
		delay_ms(5000);
		OLED_Clear();	                          //清屏
		
		OLED_ShowCHinese(10,0,0);//电            //显示文字
		OLED_ShowCHinese(28,0,1);//子
		OLED_ShowCHinese(46,0,2);//协
		OLED_ShowCHinese(64,0,3);//会
		OLED_ShowCHinese(82,0,4);//门
		OLED_ShowCHinese(100,0,5);//禁

		f_open(file,"0:/CARD_UID.csv",FA_WRITE|FA_OPEN_ALWAYS|FA_READ);
 		f_close(file);

		while(1)
		{		
			
 			IC_test();                          //刷卡，检测，读内存卡，匹配都在这个函数
			if(Open_Door_flag==0)            
			{
				OLED_ShowCHinese(10,0,0);//电     //显示文字
				OLED_ShowCHinese(28,0,1);//子
				OLED_ShowCHinese(46,0,2);//协
				OLED_ShowCHinese(64,0,3);//会
				OLED_ShowCHinese(82,0,4);//门
				OLED_ShowCHinese(100,0,5);//禁
			}
			if(Open_Door_flag==3)    //蓝牙串口开门状态，单纯开门，灯，OLED无变化
			{
				Open_Door_flag=0;
				PFout(2)=1;							
				delay_ms(50);
				PFout(2)=0;
			}

			
		}
		
 }

 
