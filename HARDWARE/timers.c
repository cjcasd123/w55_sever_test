#include "timers.h"
#include "stm32f4xx_tim.h"

unsigned int Timer2_Counter = 0;            // Timer2��ʱ����������(ms)
unsigned int W5500_Send_Delay_Counter = 0;  // W5500������ʱ��������(ms)

uint32	ms = 0;															  	/*�������*/
uint32	dhcp_time = 0;															  	/*DHCP���м���*/
vu8	    ntptimer = 0;

/*
    ʱ����㹫ʽ��Tout=((arr+1)*(psc+1))/Ft us
*/
void Timer2_Init_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;						//����NVIC��ʼ���ṹ��

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//�����ж�������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�����������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//������Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ��NVIC
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//ʹ��Timer2ʱ��

	TIM_TimeBaseStructure.TIM_Period = 9;						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ(������10Ϊ1ms)
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ(10KHz�ļ���Ƶ��)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�:TDTS = TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 				//ʹ��TIM2ָ�����ж�

	TIM_Cmd(TIM2, ENABLE);  									//ʹ��TIMx����
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		Timer2_Counter++;
		W5500_Send_Delay_Counter++;
		// ����DHCP��ʱ��
		ms++;
		if (ms >= 1000)
		{
			ms = 0;
			dhcp_time++;																					/*DHCP��ʱ��1S*/
			//  #ifndef	__NTP_H__
			//  ntptimer++;																						/*NTP����ʱ���1S*/
			// #endif
		}
	}
}


void Delay(unsigned int d)
{
	Timer2_Counter = 0;
	while (Timer2_Counter < d);
}



