#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h" 
#include "usart3.h"
#include "common.h" 
#include "adc.h"
#include "smog.h"
#include "beep.h"
#include "relay.h"
#include "dht11.h"



 int main(void)
 {	 
	u8 key,fontok=0; 	    
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	//usmart_dev.init(72);		//初始化USMART		
	 Adc_Init();   
	 BEEP_Init();	
	 DHT11_Init();                                    //DHT11初始化  PC13
	 Smog_Init();	
	 RELAY_Init(); 
	 
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD   
	usart3_init(115200);		//初始化串口3 
	key=KEY_Scan(0);  
 	while(DHT11_Init())	//DHT11初始化	
	{
		 printf("DHT11 Error!\r\n");
		delay_ms(500);
	}
		atk_8266_test();		//进入ATK_ESP8266测试
}


















