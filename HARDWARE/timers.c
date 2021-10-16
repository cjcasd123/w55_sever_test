#include "timers.h"
#include "stm32f4xx_tim.h"

unsigned int Timer2_Counter = 0;            // Timer2定时器计数变量(ms)
unsigned int W5500_Send_Delay_Counter = 0;  // W5500发送延时计数变量(ms)

uint32	ms = 0;															  	/*毫秒计数*/
uint32	dhcp_time = 0;															  	/*DHCP运行计数*/
vu8	    ntptimer = 0;

/*
    时间计算公式：Tout=((arr+1)*(psc+1))/Ft us
*/
void Timer2_Init_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;						//定义NVIC初始化结构体

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//设置中断向量号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//设置抢先优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//设置响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能NVIC
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//使能Timer2时钟

	TIM_TimeBaseStructure.TIM_Period = 9;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值(计数到10为1ms)
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					//设置用来作为TIMx时钟频率除数的预分频值(10KHz的计数频率)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//设置时钟分割:TDTS = TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 				//使能TIM2指定的中断

	TIM_Cmd(TIM2, ENABLE);  									//使能TIMx外设
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		Timer2_Counter++;
		W5500_Send_Delay_Counter++;
		// 设置DHCP的时间
		ms++;
		if (ms >= 1000)
		{
			ms = 0;
			dhcp_time++;																					/*DHCP定时加1S*/
			//  #ifndef	__NTP_H__
			//  ntptimer++;																						/*NTP重试时间加1S*/
			// #endif
		}
	}
}


void Delay(unsigned int d)
{
	Timer2_Counter = 0;
	while (Timer2_Counter < d);
}



