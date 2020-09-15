#include "common.h"
#include "adc.h"
#include "smog.h"
#include "beep.h"
#include "relay.h"
#include "dht11.h"

//ATK-ESP8266 WIFI AP����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
//wifi

u8 temperature=0; //�¶� 	    
u8 humidity=0;    //ʪ��
u16 smogData=0; //���� 
char* beepStateHexStr;
char* relayStateHexStr;
char* cmdStr="AT+CIPSEND=0,25";

u8 atk_8266_wifiap_test(void)
{
	u8 netpro=0;	//����ģʽ
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP����
	u8 *p;
	char *pdata;
	u16 i;
   unsigned char data[256];    //��д��������
  char *d="+IPD,0,3:#";			
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//����״̬
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�

	
PRESTA:
	netpro=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]);	//ѡ������ģʽ
	if(netpro&0X02)   //UDP
	{
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				LCD_ShowString(30,40,210,24,20,"ATK-ESP WIFI-AP Module"); 
	    	LCD_ShowString(30,70,200,16,16,"UDP Waiting...");
				if(atk_8266_ip_set("WIFI-AP Զ��UDP IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
				sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������
				atk_8266_send_cmd("AT+CIPMUX=0","OK",100);  //������ģʽ
				LCD_Clear(WHITE);
				while(atk_8266_send_cmd(p,"OK",500));
			
	}
	else     //TCP
	{
		if(netpro&0X01)     //TCP Client    ͸��ģʽ����
		{
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
				LCD_ShowString(30,40,210,24,20,"ATK-ESP WIFI-AP Module"); 
	    	LCD_ShowString(30,70,200,16,16,"TCP Client Waiting...");
			
			if(atk_8266_ip_set("WIFI-AP IP Settings",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������
			while(atk_8266_send_cmd(p,"OK",200))
			{
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				key=KEY_Scan(0);
				if(key==WKUP_PRES)goto PRESTA;
			}	
			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��		
			}
			else					//TCP Server
			{
					LCD_Clear(WHITE);
					POINT_COLOR=RED;
					LCD_ShowString(30,40,210,24,20,"ATK-ESP WIFI-AP Module"); 
					LCD_ShowString(30,70,200,16,16,"TCP Server Waiting...");
					atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
					sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
					atk_8266_send_cmd(p,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086
			}
	}
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
			LCD_Fill(30,50,239,50+12,WHITE);			//���֮ǰ����ʾ
			//Show_Str(30,50,200,16,"����ATK-ESPģ��ɹ�!",12,0);
    	LCD_ShowString(30,40,200,16,16,"ATK-ESP Succeess!");
			delay_ms(200);
		//	Show_Str(30,50,200,16,"WK_UP:�˳�����  KEY0:��������",12,0);
			LCD_Fill(30,80,239,80+12,WHITE);
			atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
			sprintf((char*)p,"IP:%s Port:%s",ipbuf,(u8*)portnum);
			LCD_ShowString(30,90,200,16,16,p);	//��ʾIP��ַ�Ͷ˿�	
			LCD_ShowString(30,110,200,16,16,"state:");	//����״̬
			LCD_ShowString(30,130,200,16,16,"model:");//����ģʽ
			LCD_ShowString(30,150,200,16,16,"send:");//��������
			LCD_ShowString(30,180,200,16,16,"rece:");//��������

			atk_8266_wificonf_show(30,240,"Connect WIFI,Please:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password); //��ʾ������Ϣ
			POINT_COLOR=BLUE;
			LCD_ShowString(30+60,130,200,16,16,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro]);	//����ģʽ
	
	
			USART3_RX_STA=0;
			
			while(1)
			{
				delay_ms(200);
				
				DHT11_Read_Data(&temperature,&humidity);     //��ȡ��ʪ��ֵ				
				printf("Temp:%d�� Humi:%d\r\n",temperature,humidity);	
				smogData=Smog_Trans_Concentration();  
				if(humidity>=95)                  //ʪ�� ����95%���Ͼʹ򿪼̵�����ģ�⽵ʪ�豸
				{
					 delay_ms(100);//��ʱ300ms
					// RELAY = 0 ;                         //�͵�ƽ������
					 relayStateHexStr="ON ";
				}else 
				{
						delay_ms(100);//��ʱ300ms
					//	RELAY = 1 ;
						relayStateHexStr="OFF";
				}	
				 
				if(temperature>=34||smogData>=360)  //��������������
					{
						 BEEP=1;   
						 delay_ms(500);//��ʱ500ms 
						 BEEP=0;		
						 delay_ms(500);//��ʱ500ms
					 	beepStateHexStr="ON ";
					}else 
					{
						 BEEP=0;	
						beepStateHexStr="OFF";
					}	
					
	 		if(USART3_RX_STA&0X8000)		//���յ�һ��������
			{ 			   
				rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
				USART3_RX_BUF[rlen]=0;		//��ӽ����� 
				printf("%s\r\n",USART3_RX_BUF);	//���͵�����  

				sprintf((char*)p,"%d",rlen);//���յ����ֽ��� 
				delay_ms(200);

        if(rlen==12)
				{
					printf("��\r\n");	//���͵�����  
					LED1= 0 ;
				}else
        {
					printf("��\r\n");	//���͵�����  
					LED1 = 1 ;
				}				

				POINT_COLOR=BRED;
				 LCD_ShowString(30+40,180,200,16,16,p);//��ʾ���յ������ݳ���	
				POINT_COLOR=BLUE;
				 LCD_ShowString(30,200,200,16,12,USART3_RX_BUF);//��ʾ���յ������� 
				USART3_RX_STA=0;
			 }  		
			
				key=KEY_Scan(0);
				if(key==WKUP_PRES)			//WK_UP �˳�����		 
				{  
					res=0;
					atk_8266_quit_trans();	//�˳�͸��
					atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //�ر�͸��ģʽ
					break;												 
				}
				else
		      //if(key==KEY0_PRES)	//KEY0 �������� 
				{
				
					if((netpro==3)||(netpro==2))   //UDP
					{
						sprintf((char*)p,"UDP%s%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
						atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //����ָ�����ȵ�����
						delay_ms(200);
						atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
						timex=100;
					}
					else if((netpro==1))   //TCP Client
					{
						atk_8266_quit_trans();
						atk_8266_send_cmd("AT+CIPSEND","OK",20);       //��ʼ͸��
						sprintf((char*)p,"TCP Client%s%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
						u3_printf("%s",p);
						timex=100;
					}
					else    //TCP Server
					{
						sprintf((char*)p,"%d%d%04d%s%s",temperature,humidity,smogData,relayStateHexStr,beepStateHexStr);//���ݰ�
					  LCD_ShowString(30+50,150,200,16,16,p);//��������
						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
						printf("\r\n�ϱ������ݰ�:%s\r\n",p);
						atk_8266_send_data(p,"OK",200);  //����ָ�����ȵ�����	
						delay_ms(300);
						timex=100;
					}
				}
			
			if(timex)timex--;
			if(timex==1)
				LCD_Fill(30,220,239,339,WHITE);
			t++;
			delay_ms(5);
				
			
			if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
			{
				constate=atk_8266_consta_check();//�õ�����״̬
				if(constate=='+')
				{
					LCD_ShowString(30+60,110,200,16,16,"Connection OK");	//����״̬
				}
				else 
				{
					 LCD_ShowString(30+60,110,200,16,16,"Connection NO");	//����״̬
				}			
				t=0;
			}
			if((t%20)==0)LED0=!LED0;
			
			atk_8266_at_response(1);
				
     }
			

	 
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	return res;		
} 







