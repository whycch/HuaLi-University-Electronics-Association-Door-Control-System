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

//ƽ̨��STM32F103ZET6��Сϵͳ���İ�
//���ߣ����ԣ
//�ص㣺�㶫��ҵ��ѧ����ѧԺ
//���ܣ�RC522��Ƶģ���ȡ��Ƶ����UID�������շ��������ʽת�룬��ת���ı������ڴ濨CARD_UID.csv�ļ��еĿ���ƥ��
//�ο�������ԭ��@ALIENTEK
//���ڣ�2019/6/1

/*********************************************
�������ܣ���������
������len  �����ĳ���
����ֵ:ִ�н��
*********************************************/
u8 SD_read_dat(u16 len)
{
	u16 t;
	u8 res=0;
	u16 tlen=0;
	printf("\r\nRead file data is:\r\n");

		res=f_read(file,SD_buf,len,&br);
		if(res)	//�����ݳ�����
		{
			printf("\r\nRead Error:%d\r\n",res);   
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)printf("%c",SD_buf[t]); 
		}	 
	if(tlen)printf("\r\nReaded data len:%d\r\n",tlen);//���������ݳ���
	printf("Read data over\r\n");	 
	return res;
}


/*********************************************
  * @brief  IC���Ժ���
  * @param  ��
  * @retval ��
*********************************************/
 u8	Open_Door_flag=0;   //���ű�־λ   0 �޶���״̬  1ˢ���ɹ�������  2ˢ��ʧ�ܣ�������

void IC_test ( void )
{
	char cStr [ 30 ];
	char UID_str [ 10 ];
  uint8_t ucArray_ID [ 4 ];    /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/                                                                                         
	uint8_t ucStatusReturn;      /*����״̬*/ 
	char buffer[20];
	u8 i;
	u32 crad_num=0;
	u32 len;
	u8 Name[20],Num[20],Card_id[20],flag[20];
	
	u32 k,j;
	u8 flag_mode=0;

		
    /*Ѱ��*/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )  
       /*��ʧ���ٴ�Ѱ��*/
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                

		if ( ucStatusReturn == MI_OK  )
		{

      /*����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				Beed();    //��⵽�п���BEEPһ����ʾ
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
				
				for(i=4;i>0;i--)                             //������UIDת��Ϊ���ţ���Ҫ��Ϊ�˷��ϻ���ѧԺ�ķ������š�
				{                                            //ת�ù�ʽΪ��UID�루8λʮ�������룩ѭ�����ư�λ4�Σ��õ���UID��תΪʮ����
					crad_num+=(u32)(ucArray_ID[i-1])<<(8*(i-1));   //���磺����ԭUID�룺FA1B904B --��ѭ�����ư�λ4�κ� --����UID�룺4B901BFA
				}                                                //      ��UID��תΪʮ���ƺ�1267735546 (���ſ����ó���Ա��������ʮ������תʮ���ơ�����)
				sprintf( buffer,"%u",crad_num );  //���ｫʮ��������תΪ�ַ�������buffer����
        crad_num=0;			                  //���ת����ʮ���Ʊ�����Ϊ���´�ˢ��ת���Ǵ�0��ʼ����
        
				printf ( "%s\r\n",cStr );     //������ʾ
				printf ( "The Card Number is:%s\r\n",buffer );     //������ʾ		
				OLED_ShowString(0,5,"UID:");
				OLED_ShowString(0,6,"Code:");
				OLED_ShowString(40,5,UID_str);  //OLED��ʾ����UID
				OLED_ShowString(45,6,buffer);  //OLED��ʾ����UID	
				len=f_size(file);			
				f_open(file,"0:/CARD_UID.csv",FA_OPEN_ALWAYS|FA_READ);			
				SD_read_dat(len);            //��ȡ�ڴ濨�е�"0:/CARD_UID.csv"�ļ�����������SD_buf�в��ô��ڴ�ӡ����������				
/******************************************************************************************************************/	
				for(k=0;k<len;k++)           //ƥ������SD_buf�������ڴ濨"0:/CARD_UID.csv"�ļ�����Ĵ洢�Ŀ��ź͵�ǰˢ���Ŀ���
				{															//�洢��ʽΪ ���,����,����,��־λ  (ע��","ΪӢ���ַ�����)
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
					
					if(strcmp(Card_id,buffer)==0)                   //ƥ��ɹ����������0�����ҵ���LED1
					{			
								
								printf("\r\nOpen Door Success ��ӭ��������Э��\r\n");
								OLED_ShowString(0,1,"---------------");
								OLED_ShowCHinese(28,3,20);//ˢ          //OLED��ʾ����
								OLED_ShowCHinese(46,3,21);//��
								OLED_ShowCHinese(64,3,22);//��
								OLED_ShowCHinese(82,3,23);//��
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
				f_close(file);                 //�����ļ�ϵͳ������Ҫ�ص�
	
				if(Open_Door_flag==0)          //ƥ�䲻�ɹ��������ƥ��ͬʱ�ص�LED1
				{
						Open_Door_flag=2;
						LED1=1;	
						PFout(2)=0;
						printf("\r\nOpen Door Error ˢ��ʧ��\r\n");		
						OLED_ShowString(0,1,"---------------");
						OLED_ShowCHinese(28,3,20);//ˢ              //OLED��ʾ����
						OLED_ShowCHinese(46,3,21);//��
						OLED_ShowCHinese(64,3,24);//ʧ
						OLED_ShowCHinese(82,3,25);//��	
						OLED_ShowString(0,4,"---------------");
						
				}                
							
			}	

		}
		
		
}




