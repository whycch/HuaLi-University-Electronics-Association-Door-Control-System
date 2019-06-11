#include "RC522.H"
#include "rc522_config.h"
#include "rc522_function.h"
#include "usart.h"
#include "beep.h"
#include "oled.h"
#include "delay.h"
#include "led.h"
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h" 
#include "string.h"

//平台：STM32F103ZET6最小系统核心板
//作者：吴宏裕
//地点：广东工业大学华立学院
//功能：RC522射频模块读取射频卡的UID，并按照饭卡编码格式转码，将转码后的编码与内存卡CARD_UID.csv文件中的卡号匹配
//参考：正点原子@ALIENTEK
//日期：2019/6/1

/*********************************************
函数功能：读出数据
变量：len  读出的长度
返回值:执行结果
*********************************************/
u8 SD_read_dat(u16 len)
{
	u16 t;
	u8 res=0;
	u16 tlen=0;
	printf("\r\nRead file data is:\r\n");

		res=f_read(file,SD_buf,len,&br);
		if(res)	//读数据出错了
		{
			printf("\r\nRead Error:%d\r\n",res);   
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",SD_buf[t]); 
		}	 
	if(tlen)printf("\r\nReaded data len:%d\r\n",tlen);//读到的数据长度
	printf("Read data over\r\n");	 
	return res;
}


/*********************************************
  * @brief  IC测试函数
  * @param  无
  * @retval 无
*********************************************/
 u8	Open_Door_flag=0;   //开门标志位   0 无动作状态  1刷卡成功，开门  2刷卡失败，不开门

void IC_test ( void )
{
	char cStr [ 30 ];
	char UID_str [ 10 ];
  uint8_t ucArray_ID [ 4 ];    /*先后存放IC卡的类型和UID(IC卡序列号)*/                                                                                         
	uint8_t ucStatusReturn;      /*返回状态*/ 
	char buffer[20];
	u8 i;
	u32 crad_num=0;
	u32 len;
	u8 Name[20],Num[20],Card_id[20],flag[20];
	
	u32 k,j;
	u8 flag_mode=0;

		
    /*寻卡*/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )  
       /*若失败再次寻卡*/
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                

		if ( ucStatusReturn == MI_OK  )
		{

      /*防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				Beed();    //检测到有卡后，BEEP一声提示
				sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X",
                  ucArray_ID [ 0 ],
                  ucArray_ID [ 1 ],
                  ucArray_ID [ 2 ],
                  ucArray_ID [ 3 ] );
				sprintf ( UID_str, "%02X%02X%02X%02X",
                  ucArray_ID [ 0 ],
                  ucArray_ID [ 1 ],
                  ucArray_ID [ 2 ],
                  ucArray_ID [ 3 ] );
				
				for(i=4;i>0;i--)                             //将卡的UID转换为卡号，主要是为了符合华立学院的饭卡卡号。
				{                                            //转好公式为，UID码（8位十六进制码）循环左移八位4次，得到新UID码转为十进制
					crad_num+=(u32)(ucArray_ID[i-1])<<(8*(i-1));   //例如：饭卡原UID码：FA1B904B --》循环左移八位4次后 --》新UID码：4B901BFA
				}                                                //      新UID码转为十进制后：1267735546 (不信可以用程序员计算器“十六进制转十进制”看看)
				sprintf( buffer,"%u",crad_num );  //这里将十进制数字转为字符串，用buffer储存
        crad_num=0;			                  //清除转化的十进制变量，为了下次刷卡转化是从0开始计算
        
				printf ( "%s\r\n",cStr );     //串口显示
				printf ( "The Card Number is:%s\r\n",buffer );     //串口显示		
				OLED_ShowString(0,5,"UID:");
				OLED_ShowString(0,6,"Code:");
				OLED_ShowString(40,5,UID_str);  //OLED显示卡的UID
				OLED_ShowString(45,6,buffer);  //OLED显示卡的UID	
				len=f_size(file);			
				f_open(file,"0:/CARD_UID.csv",FA_OPEN_ALWAYS|FA_READ);			
				SD_read_dat(len);            //读取内存卡中的"0:/CARD_UID.csv"文件，存在数组SD_buf中并用串口打印出里面内容				
/******************************************************************************************************************/	
				for(k=0;k<len;k++)           //匹配数组SD_buf读出来内存卡"0:/CARD_UID.csv"文件里面的存储的卡号和当前刷卡的卡号
				{															//存储格式为 序号,名字,卡号,标志位  (注意","为英文字符都号)
					if(flag_mode==0)           
						Num[j]=SD_buf[k];         
					else if(flag_mode==1)
						Name[j]=SD_buf[k];
					else if(flag_mode==2)
						Card_id[j]=SD_buf[k];
					else if(flag_mode==3)
						flag[j]=SD_buf[k];	
					
					if(Num[j]==','&&flag_mode==0)
					{
						flag_mode=1;
						Num[j]='\0';
						j=0;	
					}
					else if(Name[j]==','&&flag_mode==1)
					{
						flag_mode=2;
						Name[j]='\0';
						j=0;
					}
					else if(Card_id[j]==','&&flag_mode==2)
					{
						flag_mode=3;
						Card_id[j]='\0';
						j=0;
					}
					else if(flag[j]=='\n'&&flag_mode==3)
					{ 
						flag_mode=0;
						flag[j-1]='\0';
						j=0;
					}		
					else 
						j++;
					
					if(strcmp(Card_id,buffer)==0)                   //匹配成功，串口输出0，并且点亮LED1
					{			
								
								printf("\r\nOpen Door Success 欢迎来到电子协会\r\n");
								OLED_ShowString(0,1,"---------------");
								OLED_ShowCHinese(28,3,20);//刷          //OLED显示中文
								OLED_ShowCHinese(46,3,21);//卡
								OLED_ShowCHinese(64,3,22);//成
								OLED_ShowCHinese(82,3,23);//功
								OLED_ShowString(0,4,"---------------");
								Open_Door_flag=1;		
								PFout(2)=1;							
								delay_ms(50);
								PFout(2)=0;
								LED1=0;
						break;
					}
				}
/******************************************************************************************************************/	
				f_close(file);                 //开了文件系统，用完要关掉
	
				if(Open_Door_flag==0)          //匹配不成功，则继续匹配同时关掉LED1
				{
						Open_Door_flag=2;
						LED1=1;	
						PFout(2)=0;
						printf("\r\nOpen Door Error 刷卡失败\r\n");		
						OLED_ShowString(0,1,"---------------");
						OLED_ShowCHinese(28,3,20);//刷              //OLED显示中文
						OLED_ShowCHinese(46,3,21);//卡
						OLED_ShowCHinese(64,3,24);//失
						OLED_ShowCHinese(82,3,25);//败	
						OLED_ShowString(0,4,"---------------");
						
				}                
							
			}	

		}
		
		
}




