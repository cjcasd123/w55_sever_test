#ifndef _TIMERS_H_ 
#define _TIMERS_H_

#include "types.h"

extern uint32  dhcp_time;

void Timer2_Init_Config(void);		//Timer2初始化配置
void Delay(unsigned int d);			//延时函数(ms)


#endif
