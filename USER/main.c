
#include "sys.h"
#include "delay.h"
#include "w5500_conf.h"
#include "w5500.h"
#include "utility.h"
#include "usart.h"
#include "timers.h"
#include "dhcp.h"
#include "httputil.h"
#include "netbios.h"
#include <string.h>

/*
   重新建立新的服务端
    1.建立后端服务器（局域网NETbios/DNS）
	2.建立客循环访问(服务器的连续性访问)
	3.建立后端存储（SD卡）
   内部资源的分配
	W5500IO口设置



*/

uint8 table1[] = "hello world!";
// di ceng chu shi hua 
void p_system_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	Timer2_Init_Config();		//Timer2初始化配置
	/* 初始化W5500的GPIO */
	gpio_for_w5500_config();
	//gpio_w5500_config();

}

// W5500 chu shi hua
void p_all_init(void)
{
	p_system_init();
	reset_w5500();											/*硬复位W5500*/
	set_w5500_mac();										/*配置MAC地址*/

	//set_w5500_ip();											/*配置IP地址*/

	socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/
	printf(" 网络已完成初始化……\r\n");
}


int main(void)
{
	p_all_init();
#if 0
	printf("OK1\r\n");
	Delay(3000);
	printf("OK2\r\n");
	while (1)
	{
		Server_Task_A();
		delay_ms(2000);
	}
#endif

#if 1
	while (1)
	{
		// 内网的平台访问（建立在路由器的内网（私有云））
	  // 建立服务器  ---- 前端（网页的显示）
	  // 1.获取路由器中的随机IP
	  // 2.建立HTTP通信下的网页显示和执行功能
	  // 3.建立Netbios下的局域网的域名访问
		while (1)
		{
			do_dhcp();
			delay_ms(1000);
			if (dhcp_sign == 5)
			{
				printf("已经获取内网IP地址进行外网的通信\r\n");
				dhcp_sign = 0;
				break;
			}
		}
		while (1)
		{
			printf("开始服务器\r\n");
			do_https();
			do_netbios();
			delay_ms(1000);
			delay_ms(1000);
			// 当设备接收到网页传回来的数据时会进入（执行相关的数据转变或者执行控制功能）
			if (reboot_flag == 1)
			{
				printf("测试结束执行其他命令\r\n");
				while (1);
			}
			// 再重新进行配置页面
			//reboot();
		}



	}
#endif
}


