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
 ALIENTEK STM32F103������ ��չʵ��5
 ATK-RM04 WIFIģ�����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


 int main(void)
 {	 
	u8 key,fontok=0; 	    
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	//usmart_dev.init(72);		//��ʼ��USMART		
	 Adc_Init();   
	 BEEP_Init();	
	 DHT11_Init();                                    //DHT11��ʼ��  PC13
	 Smog_Init();	
	 RELAY_Init(); 
	 
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD   
	//W25QXX_Init();				//��ʼ��W25Q128
//	tp_dev.init();				//��ʼ��������
	usart3_init(115200);		//��ʼ������3 
	key=KEY_Scan(0);  
 	while(DHT11_Init())	//DHT11��ʼ��	
	{
		 printf("DHT11 Error!\r\n");
		delay_ms(500);
	}
		atk_8266_test();		//����ATK_ESP8266����
}


















