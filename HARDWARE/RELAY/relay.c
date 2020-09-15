#include "relay.h"


//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void RELAY_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA4端口时钟
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.2
 GPIO_SetBits(GPIOA,GPIO_Pin_4);						 //PA4 输出高
 //GPIO_ResetBits(GPIOB,GPIO_Pin_3);

}




//STM32的部分IO口可以容忍5V，部分IO口只能是3.3V容忍。

//到底哪些能够容忍，查看数据手册，引脚标注”FT"的是可以容忍5V的。

//比如：STM32F103xCDE_DS_CH_V5.pdf  在我们光盘“8，STM32参考资料\STM32中文数据手册"下面

//STM32F4的IO电平兼容性问题，STM32F4的绝大部分IO口，都兼容5V，

//至于到底哪些是兼容5V的，请看STM32F40x的数据手册（注意是数据手册，不是中文参考手册！！），

//见 表：Table 6 STM32F40x pin and ball definitions，凡是有FT/FTf标志的，都是兼容5V电平 的IO口，可以直接接5V的外设

//（注意：如果引脚设置的是模拟输入模式，则不能接5V！)，

//凡是不是FT/FTf标志的，大家都不要 接5V了，可能烧坏MCU。










