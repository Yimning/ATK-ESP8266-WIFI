#include "relay.h"


//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void RELAY_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA4�˿�ʱ��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.2
 GPIO_SetBits(GPIOA,GPIO_Pin_4);						 //PA4 �����
 //GPIO_ResetBits(GPIOB,GPIO_Pin_3);

}




//STM32�Ĳ���IO�ڿ�������5V������IO��ֻ����3.3V���̡�

//������Щ�ܹ����̣��鿴�����ֲᣬ���ű�ע��FT"���ǿ�������5V�ġ�

//���磺STM32F103xCDE_DS_CH_V5.pdf  �����ǹ��̡�8��STM32�ο�����\STM32���������ֲ�"����

//STM32F4��IO��ƽ���������⣬STM32F4�ľ��󲿷�IO�ڣ�������5V��

//���ڵ�����Щ�Ǽ���5V�ģ��뿴STM32F40x�������ֲᣨע���������ֲᣬ�������Ĳο��ֲᣡ������

//�� ��Table 6 STM32F40x pin and ball definitions��������FT/FTf��־�ģ����Ǽ���5V��ƽ ��IO�ڣ�����ֱ�ӽ�5V������

//��ע�⣺����������õ���ģ������ģʽ�����ܽ�5V��)��

//���ǲ���FT/FTf��־�ģ���Ҷ���Ҫ ��5V�ˣ������ջ�MCU��










