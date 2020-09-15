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


/************************************************
 ALIENTEK STM32F103开发板 扩展实验5
 ATK-RM04 WIFI模块测试实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


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
	//W25QXX_Init();				//初始化W25Q128
//	tp_dev.init();				//初始化触摸屏
	usart3_init(115200);		//初始化串口3 
	key=KEY_Scan(0);  
 	while(DHT11_Init())	//DHT11初始化	
	{
		 printf("DHT11 Error!\r\n");
		delay_ms(500);
	}
		atk_8266_test();		//进入ATK_ESP8266测试
}


















