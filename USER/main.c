
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
   ���½����µķ����
    1.������˷�������������NETbios/DNS��
	2.������ѭ������(�������������Է���)
	3.������˴洢��SD����
   �ڲ���Դ�ķ���
	W5500IO������



*/

uint8 table1[] = "hello world!";
// di ceng chu shi hua 
void p_system_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(115200);
	Timer2_Init_Config();		//Timer2��ʼ������
	/* ��ʼ��W5500��GPIO */
	gpio_for_w5500_config();
	//gpio_w5500_config();

}

// W5500 chu shi hua
void p_all_init(void)
{
	p_system_init();
	reset_w5500();											/*Ӳ��λW5500*/
	set_w5500_mac();										/*����MAC��ַ*/

	//set_w5500_ip();											/*����IP��ַ*/

	socket_buf_init(txsize, rxsize);		/*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	printf(" ��������ɳ�ʼ������\r\n");
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
		// ������ƽ̨���ʣ�������·������������˽���ƣ���
	  // ����������  ---- ǰ�ˣ���ҳ����ʾ��
	  // 1.��ȡ·�����е����IP
	  // 2.����HTTPͨ���µ���ҳ��ʾ��ִ�й���
	  // 3.����Netbios�µľ���������������
		while (1)
		{
			do_dhcp();
			delay_ms(1000);
			if (dhcp_sign == 5)
			{
				printf("�Ѿ���ȡ����IP��ַ����������ͨ��\r\n");
				dhcp_sign = 0;
				break;
			}
		}
		while (1)
		{
			printf("��ʼ������\r\n");
			do_https();
			do_netbios();
			delay_ms(1000);
			delay_ms(1000);
			// ���豸���յ���ҳ������������ʱ����루ִ����ص�����ת�����ִ�п��ƹ��ܣ�
			if (reboot_flag == 1)
			{
				printf("���Խ���ִ����������\r\n");
				while (1);
			}
			// �����½�������ҳ��
			//reboot();
		}



	}
#endif
}


