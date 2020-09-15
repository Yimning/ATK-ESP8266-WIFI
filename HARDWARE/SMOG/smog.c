#include "smog.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

//初始化烟雾传感器
void Smog_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTF时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			//PC0 anolog输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	Adc_Init();
}
//读取烟雾传感器的电压值
u16 Smog_Get_Vol(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<SMOG_READ_TIMES;t++)
	{
		temp_val+=Get_Adc(SMOG_ADC_CHX);	//读取ADC值
		delay_ms(5);
	}
	temp_val/=SMOG_READ_TIMES;//得到平均值
	printf("Smog_ADC_Val: %fV\r\n", 3.3/4096*temp_val); //ADC的数字量 ，无单位   若换单位为v 则需要*3.3/4096  4096为12为的ADC最大的值。

	return (u16)temp_val;
}
//分析从烟雾传感器获取的电压值，通过公式计算出可燃气体的浓度值
//设Rs为传感器的体电阻，其中气体浓度上升，将导致Rs下降。而Rs的下降则会导致，MQ-2的4脚、6脚对地输出的电压增大。
//所以气体浓度增大，其输出的电压也会增大。因Rs在不同气体中有不同的浓度值，所以该函数仅作为参考.
u16 Smog_Trans_Concentration(void)
{
    u16 ulVal = 0;
	  u16 smoke_val=0;
    u16 temp_val = Smog_Get_Vol();
   	u16 Rs;
	  //Rs = SMOG_PIN46_R*(4096.0/temp_val - 1);
	  Rs = ((Smog_Get_Vol() *(3.3 / 4096)-1)/0.1)*1000;
	  // Rs = SMOG_PIN46_R*(3.3/4096.0*temp_val - 1);
	 // printf("Smog_Rs_Val:%d\r\n", Rs);
	   smoke_val+=((3.3/4096*temp_val)-1)*300;

	  printf("烟雾浓度:%dppm\r\n", smoke_val);
	 ulVal = Rs;//这里填写公式根据Rs计算气体浓度
	
    return smoke_val;
}

//4、正常环境下（即没有被测气体），传感器以模拟量输出时，OUT端的电压在1v左右，
//当传感器检测到被测气体时，电压每升高0.1v，实际被测气体的浓度增加200ppm。
//（简单的说：1ppm = 1mg/kg = 1mg/L = 1x10-6常用来表示气体浓度，或者溶液浓度。）

