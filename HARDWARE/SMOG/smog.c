#include "smog.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

//��ʼ����������
void Smog_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			//PC0 anolog����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	Adc_Init();
}
//��ȡ���������ĵ�ѹֵ
u16 Smog_Get_Vol(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<SMOG_READ_TIMES;t++)
	{
		temp_val+=Get_Adc(SMOG_ADC_CHX);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=SMOG_READ_TIMES;//�õ�ƽ��ֵ
	printf("Smog_ADC_Val: %fV\r\n", 3.3/4096*temp_val); //ADC�������� ���޵�λ   ������λΪv ����Ҫ*3.3/4096  4096Ϊ12Ϊ��ADC����ֵ��

	return (u16)temp_val;
}
//����������������ȡ�ĵ�ѹֵ��ͨ����ʽ�������ȼ�����Ũ��ֵ
//��RsΪ������������裬��������Ũ��������������Rs�½�����Rs���½���ᵼ�£�MQ-2��4�š�6�ŶԵ�����ĵ�ѹ����
//��������Ũ������������ĵ�ѹҲ��������Rs�ڲ�ͬ�������в�ͬ��Ũ��ֵ�����Ըú�������Ϊ�ο�.
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

	  printf("����Ũ��:%dppm\r\n", smoke_val);
	 ulVal = Rs;//������д��ʽ����Rs��������Ũ��
	
    return smoke_val;
}

//4�����������£���û�б������壩����������ģ�������ʱ��OUT�˵ĵ�ѹ��1v���ң�
//����������⵽��������ʱ����ѹÿ����0.1v��ʵ�ʱ��������Ũ������200ppm��
//���򵥵�˵��1ppm = 1mg/kg = 1mg/L = 1x10-6��������ʾ����Ũ�ȣ�������ҺŨ�ȡ���

