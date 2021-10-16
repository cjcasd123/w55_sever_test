/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		����MCU����ֲW5500������Ҫ�޸ĵ��ļ�������W5500��MAC��IP��ַ
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>

#include "w5500_conf.h"
#include "utility.h"
#include "w5500.h"
#include "usart.h"
//#include "stm32f10x_spi.h"
#include "delay.h"
#include "socket.h"
#include "dhcp.h"

CONFIG_MSG  ConfigMsg;																	/*���ýṹ��*/
EEPROM_MSG_STR EEPROM_MSG;															/*EEPROM�洢��Ϣ�ṹ��*/

/*IP���÷���ѡ��������ѡ��*/
//uint8	ip_from=IP_FROM_DEFINE;			// ������ͨģʽIP	
uint8	ip_from = IP_FROM_DHCP;       // ����DHCP��IP

uint8 buff[1024];
/*����MAC��ַ,������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/* ����Ĭ��IP��Ϣ --- ���ﶨ���豸��IP��Ϣ�������ǿͻ��˺��Ƿ���� --- ��Ϊû�������Զ���IP���룩 */
// ������+�ͻ���
uint8 local_ip[4]  ={192,168,1,88};											/*����W5500Ĭ��IP��ַ*/
//uint8 local_ip[4] = { 192,168,37,4 };

uint8 subnet[4]    ={255,255,255,0};										/*����W5500Ĭ����������*/
// ������+�ͻ���
uint8 gateway[4]   ={192,168,1,1};											/*����W5500Ĭ������*/
//uint8 gateway[4] = { 192,168,37,2 };
uint8 dns_server[4]={114,114,114,114};									    /*����W5500Ĭ��DNS*/
uint16 local_port=6000;	                       					            /*���屾�ض˿�*/

#if LANeasy
/*����Զ��IP��Ϣ*/
uint8  remote_ip[4]={192,168,1,22};											/*Զ��IP��ַ*/
uint16 remote_port=5000;													/*Զ�˶˿ں�*/
#endif
#if WANeasy
/*����Զ��IP��Ϣ*/
uint8  remote_ip[4] = { 183,230,40,33 };											/*Զ��IP��ַ*/
uint16 remote_port = 80;													/*Զ�˶˿ں�*/
#endif 

uint8 dhcp_ok   = 0;													   			/*dhcp�ɹ���ȡIP*/														  	/*NPT�����*/
uint8 date1;
/**
    1
	����IP����صĲ���
	 IP MAC  ���� ����
	2
	���ַ�ʽ���������
*/
void set_w5500_ip(void)
{	
		
   /*���ƶ����������Ϣ�����ýṹ��*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);

	if(ip_from==IP_FROM_DEFINE)	
		printf(" ʹ�ö����IP��Ϣ����W5500\r\n");
	// EEPROM��ʽ  --- ʵ�ֵ��������ı���
#if 0
	/*ʹ��EEPROM�洢��IP����*/	
	if(ip_from==IP_FROM_EEPROM)
	{
    
		/*��EEPROM�ж�ȡIP������Ϣ*/
		read_config_from_eeprom();		
		
		/*�����ȡEEPROM��MAC��Ϣ,��������ã����ʹ��*/		
		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)		
		{
			printf(" IP from EEPROM\r\n");
			/*����EEPROM������Ϣ�����õĽṹ�����*/
			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
		}
		else
		{
			printf(" EEPROMδ����,ʹ�ö����IP��Ϣ����W5500,��д��EEPROM\r\n");
			write_config_to_eeprom();	/*ʹ��Ĭ�ϵ�IP��Ϣ������ʼ��EEPROM������*/
		}			
	}
#endif
	// DHCP��ʽ  ---  ����������ͨ��
#if WANeasy
	/*ʹ��DHCP��ȡIP�����������DHCP�Ӻ���*/		
	if(ip_from==IP_FROM_DHCP)								
	{
		printf("ʹ��DHCP��ʽ��IP��Ϣ����W5500\r\n");
		/*����DHCP��ȡ��������Ϣ�����ýṹ��*/
		if(dhcp_ok==1)
		{
			printf(" IP from DHCP\r\n");		 
			memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
			memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
			memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
		}
		else
		{
			printf(" DHCP�ӳ���δ����,���߲��ɹ�\r\n");
		}
	}
#endif

	/*����������Ϣ��������Ҫѡ��*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	/* ��ӡ�������ص����� */
	getSIPR (local_ip);			
	printf(" W5500 IP��ַ   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	printf(" W5500 �������� : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	printf(" W5500 ����     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}
//#if 0
/**
*@brief		����W5500��MAC��ַ
*@param		��
*@return	��
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);
	memcpy(DHCP_GET.mac, mac, 6);
}
//#endif


/**
*@brief		����W5500��GPIO�ӿ�
*@param		��
*@return	��
*/
void gpio_for_w5500_config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(WIZ_SPIx_RESET_CLK | WIZ_SPIx_INT_CLK, ENABLE);

	/* Enable SPI1 and GPIO clocks */
	/*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO,
		 SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO
		 and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
	RCC_AHB1PeriphClockCmd(WIZ_SPIx_GPIO_CLK | WIZ_SPIx_SCS_CLK, ENABLE);

	/*!< SPI_FLASH_SPI Periph clock enable */
	WIZ_SPIx_CLK_CMD(WIZ_SPIx_CLK, ENABLE);

	GPIO_PinAFConfig(WIZ_SPIx_GPIO_PORT, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(WIZ_SPIx_GPIO_PORT, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(WIZ_SPIx_GPIO_PORT, GPIO_PinSource7, GPIO_AF_SPI1);

	/*!< Configure SPI_FLASH_SPI pins: SCK */
	GPIO_InitStructure.GPIO_Pin = WIZ_SPIx_SCLK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStructure);

	/*!< Configure SPI_FLASH_SPI pins: MISO */
	GPIO_InitStructure.GPIO_Pin = WIZ_SPIx_MISO;
	GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStructure);

	/*!< Configure SPI_FLASH_SPI pins: MOSI */
	GPIO_InitStructure.GPIO_Pin = WIZ_SPIx_MOSI;
	GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStructure);

	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(WIZ_SPIx, &SPI_InitStructure);
	SPI_Cmd(WIZ_SPIx, ENABLE);

	/*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
	GPIO_InitStructure.GPIO_Pin = WIZ_SPIx_SCS;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		     /*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(WIZ_SPIx_SCS_PORT, &GPIO_InitStructure);

	/*����RESET����*/
	GPIO_InitStructure.GPIO_Pin = WIZ_RESET;					       /*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		     /*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		     /*��������ģʽΪͨ���������*/
	GPIO_Init(WIZ_SPIx_RESET_PORT, &GPIO_InitStructure);		 /*���ÿ⺯������ʼ��GPIO*/
	GPIO_SetBits(WIZ_SPIx_RESET_PORT, WIZ_RESET);
	/*����INT����*/
	GPIO_InitStructure.GPIO_Pin = WIZ_INT;						       /*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		     /*������������Ϊ100MHz*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		  /*��������ģʽΪͨ������ģ����������*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(WIZ_SPIx_INT_PORT, &GPIO_InitStructure);			 /*���ÿ⺯������ʼ��GPIO*/
}


/**
*@brief		W5500Ƭѡ�ź����ú���
*@param		val: Ϊ��0����ʾƬѡ�˿�Ϊ�ͣ�Ϊ��1����ʾƬѡ�˿�Ϊ��
*@return	��
*/
void wiz_cs(uint8_t val)
{
	if (val == LOW) 
	{
	  GPIO_ResetBits(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS); 
	}
	else if (val == HIGH)
	{
	  GPIO_SetBits(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS); 
	}
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}

/**
*@brief		W5500��λ���ú���
*@param		��
*@return	��
*/
void reset_w5500(void)
{
	GPIO_ResetBits(WIZ_SPIx_RESET_PORT, WIZ_RESET);
	delay_us(2);  
	GPIO_SetBits(WIZ_SPIx_RESET_PORT, WIZ_RESET);
	delay_ms(1600);
}

//#if 0
/* Ӳ��SPI��д�� */
uint8_t SPI_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(WIZ_SPIx, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(WIZ_SPIx, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(WIZ_SPIx, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(WIZ_SPIx);
}
//#endif
#if 0
void SPI1_Send_Byte(unsigned char dat)
{
	SPI_I2S_SendData(SPI1, dat);//д1���ֽ�����
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//�ȴ����ݼĴ�����
}
#endif
/***************************************************/
/**
*@brief		STM32 SPI1��д8λ����
*@param		dat��д���8λ����
*@return	��
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_SendByte(dat));
}

/**
*@brief		д��һ��8λ���ݵ�W5500
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data��д���8λ����
*@return	��
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
   iinchip_csoff();                              		
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
   IINCHIP_SpiSendData(data);                   
   iinchip_cson();                            
}

/**
*@brief		��W5500����һ��8λ����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   data����д��ĵ�ַ����ȡ����8λ����
*@return	��
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
   uint8 data = 0;
   iinchip_csoff();                            
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);            
   iinchip_cson();                               
   return data;    
}

/**
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ
*@param   buf��д���ַ���
*@param   len���ַ�������
*@return	len�������ַ�������
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
   uint16 idx = 0;
   if(len == 0) 
	   printf("Unexpected2 length 0\r\n");
   iinchip_csoff();                               
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
   iinchip_cson();                           
   return len;  
}

/**
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ
*@param 	buf����Ŷ�ȡ����
*@param		len���ַ�������
*@return	len�������ַ�������
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
  uint16 idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
  iinchip_csoff();                                
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    
  for(idx = 0; idx < len; idx++)                   
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                  
  return len;
}


#if LANeasy

/* �豸��Ϊ����� */
void Server_Task_A(void)
{
	uint16 len = 0;
	switch (getSn_SR(SOCK_TCPS))											            	/*��ȡsocket��״̬*/
	{
	case SOCK_CLOSED:													            /*socket���ڹر�״̬*/
		socket(SOCK_TCPS, Sn_MR_TCP, local_port, Sn_MR_ND);	                        /*��socket*/
		printf("��Socket11\r\n");
		break;

	case SOCK_INIT:														                 /*socket�ѳ�ʼ��״̬*/
		if (listen(SOCK_TCPS))											                /*socket��������*/
			printf("Socket11��������\r\n");
		else
			printf("Socket11��������ʧ��\r\n");
		break;

	case SOCK_ESTABLISHED:		/*socket�������ӽ���״̬*/
		//printf("ʶ���Ƿ�������\r\n");
		if (getSn_IR(SOCK_TCPS) & Sn_IR_CON)
		{
			setSn_IR(SOCK_TCPS, Sn_IR_CON);								          /*��������жϱ�־λ*/
		}
		len = getSn_RX_RSR(SOCK_TCPS);									            /*����lenΪ�ѽ������ݵĳ���*/
		if (len > 0)
		{
			recv(SOCK_TCPS, buff, len);/*��������Client������*/
			send(SOCK_TCPS, buff, len);/*���պ������������� */
			buff[len] = 0x00;
			printf("%s\r\n", buff);
			printf("OKA\r\n");
			printf("\r\n");
		}
		break;

	case SOCK_CLOSE_WAIT:												                /*socket���ڵȴ��ر�״̬*/
		close(SOCK_TCPS);
		printf("Socket11�ر�\r\n");
		break;

	}
}

/* �豸��Ϊ����� */
void Server_Task_B(void)
{
	uint16 len = 0;
	switch (getSn_SR(SOCK_TCPC))											            	/*��ȡsocket��״̬*/
	{
	case SOCK_CLOSED:													            /*socket���ڹر�״̬*/
		socket(SOCK_TCPC, Sn_MR_TCP, local_port, Sn_MR_ND);	                        /*��socket*/
		printf("��Socket22\r\n");
		break;

	case SOCK_INIT:														                 /*socket�ѳ�ʼ��״̬*/
		if (listen(SOCK_TCPC))											                /*socket��������*/
			printf("Socket22��������\r\n");
		else
			printf("Socket22��������ʧ��\r\n");
		break;

	case SOCK_ESTABLISHED:		/*socket�������ӽ���״̬*/
		//printf("ʶ���Ƿ�������\r\n");
		if (getSn_IR(SOCK_TCPC) & Sn_IR_CON)
		{
			setSn_IR(SOCK_TCPC, Sn_IR_CON);								          /*��������жϱ�־λ*/
		}
		len = getSn_RX_RSR(SOCK_TCPC);									            /*����lenΪ�ѽ������ݵĳ���*/
		if (len > 0)
		{
			recv(SOCK_TCPC, buff, len);/*��������Client������*/
			send(SOCK_TCPC, buff, len);/*���պ������������� */
			buff[len] = 0x00;
			printf("%s\r\n", buff);
			printf("OKB\r\n");
			printf("\r\n");
		}
		break;

	case SOCK_CLOSE_WAIT:												                /*socket���ڵȴ��ر�״̬*/
		close(SOCK_TCPC);
		printf("Socket22�ر�\r\n");
		break;

	}
}

/*
     ����һ�²�ͬ��ID�˿ں�   --- �����豸 ���Է������ڲ�ͬID�µ�����ͨ��
	 ����һ�·�������Ϊ�رղ�������
*/
/* �豸��Ϊ�ͻ��� */
void Client_Task_A(uint8 *sp, uint16 lenth)
{
	uint16 len = 0;
	uint8 sign1 = 0;
	switch (getSn_SR(SOCK_TCPC))								  				         /*��ȡsocket��״̬*/
	{
	case SOCK_CLOSED:											        		         /*socket���ڹر�״̬*/
		//socket(SOCK_TCPC, Sn_MR_TCP, local_port++, Sn_MR_ND);
		local_port=6000;
		socket(SOCK_TCPC, Sn_MR_TCP, local_port, Sn_MR_ND);
		printf("��socket\r\n");
		break;

	case SOCK_INIT:												/*socket���ڳ�ʼ��״̬*/
		connect(SOCK_TCPC, remote_ip, remote_port);                /*socket���ӷ�����*/
		printf("socket���ӷ�����\r\n");
		break;

	case SOCK_ESTABLISHED: 												             /*socket�������ӽ���״̬*/
		if (getSn_IR(SOCK_TCPC) & Sn_IR_CON)
		{
			setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*��������жϱ�־λ*/
		}
		len = getSn_RX_RSR(SOCK_TCPC);									            /*����lenΪ�ѽ������ݵĳ���*/
		if (len > 0)
		{
			recv(SOCK_TCPC, buff, len);/*��������Client������*/
			send(SOCK_TCPC, buff, len);/*���պ������������� */
			buff[len] = 0x00;
			printf("%s\r\n", buff);
			printf("OK2\r\n");
		}
		if (lenth != 0)
		{
			send(SOCK_TCPC, sp, lenth);								     	         /*��Server��������*/
			printf("0K3\r\n");

		}
		sign1 = 5;
		delay_ms(1000);
		delay_ms(1000);
		break;
#if 0
	case SOCK_CLOSE_WAIT: 											    	         /*socket���ڵȴ��ر�״̬*/
		close(SOCK_TCPC);
		printf("socket�ر�\r\n");
		break;
#endif 
	}
	if (sign1 == 5)
	{
		close(SOCK_TCPC);
	//	usart1_date = 0;
		date1 = 0;
		printf("socket�ر�\r\n");
	}
}

void Client_Task_B(uint8 *sp, uint16 lenth)
{
	uint16 len = 0;
	switch (getSn_SR(SOCK_TCPS))								  				         /*��ȡsocket��״̬*/
	{
	case SOCK_CLOSED:											        		         /*socket���ڹر�״̬*/
		//socket(SOCK_TCPC, Sn_MR_TCP, local_port++, Sn_MR_ND);
		local_port = 6000;
		socket(SOCK_TCPS, Sn_MR_TCP, local_port, Sn_MR_ND);
		printf("��socket\r\n");
		break;

	case SOCK_INIT:												/*socket���ڳ�ʼ��״̬*/
		connect(SOCK_TCPS, remote_ip, remote_port);                /*socket���ӷ�����*/
		printf("socket���ӷ�����\r\n");
		break;

	case SOCK_ESTABLISHED: 												             /*socket�������ӽ���״̬*/
		if (getSn_IR(SOCK_TCPS) & Sn_IR_CON)
		{
			setSn_IR(SOCK_TCPS, Sn_IR_CON); 							         /*��������жϱ�־λ*/
		}
		len = getSn_RX_RSR(SOCK_TCPC);									            /*����lenΪ�ѽ������ݵĳ���*/
		if (len > 0)
		{
			recv(SOCK_TCPC, buff, len);/*��������Client������*/
			send(SOCK_TCPC, buff, len);/*���պ������������� */
			buff[len] = 0x00;
			printf("%s\r\n", buff);
			printf("OK2\r\n");
		}
		if (lenth != 0)
		{
			send(SOCK_TCPS, sp, lenth);								     	         /*��Server��������*/
			printf("0K3\r\n");

		}
		delay_ms(1000);
		delay_ms(1000);
		break;

	case SOCK_CLOSE_WAIT: 											    	         /*socket���ڵȴ��ر�״̬*/
		close(SOCK_TCPS);
		printf("socket�ر�\r\n");
		break;

	}
}


#endif
