#include "common.h"
#include "adc.h"
#include "smog.h"
#include "beep.h"
#include "relay.h"
#include "dht11.h"

//ATK-ESP8266 WIFI AP测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
//wifi

u8 temperature=0; //温度 	    
u8 humidity=0;    //湿度
u16 smogData=0; //烟雾 
char* beepStateHexStr;
char* relayStateHexStr;
char* cmdStr="AT+CIPSEND=0,25";

u8 atk_8266_wifiap_test(void)
{
	u8 netpro=0;	//网络模式
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	u8 *p;
	char *pdata;
	u16 i;
   unsigned char data[256];    //代写数据数组
  char *d="+IPD,0,3:#";			
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态
	p=mymalloc(SRAMIN,32);							//申请32字节内存

	
PRESTA:
	netpro=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]);	//选择网络模式
	if(netpro&0X02)   //UDP
	{
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				LCD_ShowString(30,40,210,24,20,"ATK-ESP WIFI-AP Module"); 
	    	LCD_ShowString(30,70,200,16,16,"UDP Waiting...");
				if(atk_8266_ip_set("WIFI-AP 远端UDP IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
				sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器
				atk_8266_send_cmd("AT+CIPMUX=0","OK",100);  //单链接模式
				LCD_Clear(WHITE);
				while(atk_8266_send_cmd(p,"OK",500));
			
	}
	else     //TCP
	{
		if(netpro&0X01)     //TCP Client    透传模式测试
		{
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
				LCD_ShowString(30,40,210,24,20,"ATK-ESP WIFI-AP Module"); 
	    	LCD_ShowString(30,70,200,16,16,"TCP Client Waiting...");
			
			if(atk_8266_ip_set("WIFI-AP IP Settings",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器
			while(atk_8266_send_cmd(p,"OK",200))
			{
				LCD_Clear(WHITE);
				POINT_COLOR=RED;
				key=KEY_Scan(0);
				if(key==WKUP_PRES)goto PRESTA;
			}	
			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传		
			}
			else					//TCP Server
			{
					LCD_Clear(WHITE);
					POINT_COLOR=RED;
					LCD_ShowString(30,40,210,24,20,"ATK-ESP WIFI-AP Module"); 
					LCD_ShowString(30,70,200,16,16,"TCP Server Waiting...");
					atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
					sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
					atk_8266_send_cmd(p,"OK",20);     //开启Server模式，端口号为8086
			}
	}
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
			LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
			//Show_Str(30,50,200,16,"配置ATK-ESP模块成功!",12,0);
    	LCD_ShowString(30,40,200,16,16,"ATK-ESP Succeess!");
			delay_ms(200);
		//	Show_Str(30,50,200,16,"WK_UP:退出测试  KEY0:发送数据",12,0);
			LCD_Fill(30,80,239,80+12,WHITE);
			atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
			sprintf((char*)p,"IP:%s Port:%s",ipbuf,(u8*)portnum);
			LCD_ShowString(30,90,200,16,16,p);	//显示IP地址和端口	
			LCD_ShowString(30,110,200,16,16,"state:");	//连接状态
			LCD_ShowString(30,130,200,16,16,"model:");//连接模式
			LCD_ShowString(30,150,200,16,16,"send:");//发送数据
			LCD_ShowString(30,180,200,16,16,"rece:");//接收数据

			atk_8266_wificonf_show(30,240,"Connect WIFI,Please:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password); //显示连接信息
			POINT_COLOR=BLUE;
			LCD_ShowString(30+60,130,200,16,16,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro]);	//连接模式
	
	
			USART3_RX_STA=0;
			
			while(1)
			{
				delay_ms(200);
				
				DHT11_Read_Data(&temperature,&humidity);     //读取温湿度值				
				printf("Temp:%d℃ Humi:%d\r\n",temperature,humidity);	
				smogData=Smog_Trans_Concentration();  
				if(humidity>=95)                  //湿度 大于95%以上就打开继电器，模拟降湿设备
				{
					 delay_ms(100);//延时300ms
					// RELAY = 0 ;                         //低电平触发；
					 relayStateHexStr="ON ";
				}else 
				{
						delay_ms(100);//延时300ms
					//	RELAY = 1 ;
						relayStateHexStr="OFF";
				}	
				 
				if(temperature>=34||smogData>=360)  //蜂鸣器报警条件
					{
						 BEEP=1;   
						 delay_ms(500);//延时500ms 
						 BEEP=0;		
						 delay_ms(500);//延时500ms
					 	beepStateHexStr="ON ";
					}else 
					{
						 BEEP=0;	
						beepStateHexStr="OFF";
					}	
					
	 		if(USART3_RX_STA&0X8000)		//接收到一次数据了
			{ 			   
				rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
				USART3_RX_BUF[rlen]=0;		//添加结束符 
				printf("%s\r\n",USART3_RX_BUF);	//发送到串口  

				sprintf((char*)p,"%d",rlen);//接收到的字节数 
				delay_ms(200);

        if(rlen==12)
				{
					printf("开\r\n");	//发送到串口  
					LED1= 0 ;
				}else
        {
					printf("关\r\n");	//发送到串口  
					LED1 = 1 ;
				}				

				POINT_COLOR=BRED;
				 LCD_ShowString(30+40,180,200,16,16,p);//显示接收到的数据长度	
				POINT_COLOR=BLUE;
				 LCD_ShowString(30,200,200,16,12,USART3_RX_BUF);//显示接收到的数据 
				USART3_RX_STA=0;
			 }  		
			
				key=KEY_Scan(0);
				if(key==WKUP_PRES)			//WK_UP 退出测试		 
				{  
					res=0;
					atk_8266_quit_trans();	//退出透传
					atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //关闭透传模式
					break;												 
				}
				else
		      //if(key==KEY0_PRES)	//KEY0 发送数据 
				{
				
					if((netpro==3)||(netpro==2))   //UDP
					{
						sprintf((char*)p,"UDP%s%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
						atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //发送指定长度的数据
						delay_ms(200);
						atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
						timex=100;
					}
					else if((netpro==1))   //TCP Client
					{
						atk_8266_quit_trans();
						atk_8266_send_cmd("AT+CIPSEND","OK",20);       //开始透传
						sprintf((char*)p,"TCP Client%s%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
						u3_printf("%s",p);
						timex=100;
					}
					else    //TCP Server
					{
						sprintf((char*)p,"%d%d%04d%s%s",temperature,humidity,smogData,relayStateHexStr,beepStateHexStr);//数据包
					  LCD_ShowString(30+50,150,200,16,16,p);//发送数据
						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //发送指定长度的数据
						printf("\r\n上报的数据包:%s\r\n",p);
						atk_8266_send_data(p,"OK",200);  //发送指定长度的数据	
						delay_ms(300);
						timex=100;
					}
				}
			
			if(timex)timex--;
			if(timex==1)
				LCD_Fill(30,220,239,339,WHITE);
			t++;
			delay_ms(5);
				
			
			if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
			{
				constate=atk_8266_consta_check();//得到连接状态
				if(constate=='+')
				{
					LCD_ShowString(30+60,110,200,16,16,"Connection OK");	//连接状态
				}
				else 
				{
					 LCD_ShowString(30+60,110,200,16,16,"Connection NO");	//连接状态
				}			
				t=0;
			}
			if((t%20)==0)LED0=!LED0;
			
			atk_8266_at_response(1);
				
     }
			

	 
	myfree(SRAMIN,p);		//释放内存 
	return res;		
} 







