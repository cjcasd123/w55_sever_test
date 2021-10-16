/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		配置MCU，移植W5500程序需要修改的文件，配置W5500的MAC和IP地址
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

CONFIG_MSG  ConfigMsg;																	/*配置结构体*/
EEPROM_MSG_STR EEPROM_MSG;															/*EEPROM存储信息结构体*/

/*IP配置方法选择，请自行选择*/
//uint8	ip_from=IP_FROM_DEFINE;			// 设置普通模式IP	
uint8	ip_from = IP_FROM_DHCP;       // 设置DHCP的IP

uint8 buff[1024];
/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/* 定义默认IP信息 --- 这里定义设备的IP信息（无论是客户端和是服务端 --- 因为没有设置自动的IP填入） */
// 服务器+客户端
uint8 local_ip[4]  ={192,168,1,88};											/*定义W5500默认IP地址*/
//uint8 local_ip[4] = { 192,168,37,4 };

uint8 subnet[4]    ={255,255,255,0};										/*定义W5500默认子网掩码*/
// 服务器+客户端
uint8 gateway[4]   ={192,168,1,1};											/*定义W5500默认网关*/
//uint8 gateway[4] = { 192,168,37,2 };
uint8 dns_server[4]={114,114,114,114};									    /*定义W5500默认DNS*/
uint16 local_port=6000;	                       					            /*定义本地端口*/

#if LANeasy
/*定义远端IP信息*/
uint8  remote_ip[4]={192,168,1,22};											/*远端IP地址*/
uint16 remote_port=5000;													/*远端端口号*/
#endif
#if WANeasy
/*定义远端IP信息*/
uint8  remote_ip[4] = { 183,230,40,33 };											/*远端IP地址*/
uint16 remote_port = 80;													/*远端端口号*/
#endif 

uint8 dhcp_ok   = 0;													   			/*dhcp成功获取IP*/														  	/*NPT秒计数*/
uint8 date1;
/**
    1
	配置IP等相关的参数
	 IP MAC  网关 掩码
	2
	三种方式的填入参数
*/
void set_w5500_ip(void)
{	
		
   /*复制定义的配置信息到配置结构体*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);

	if(ip_from==IP_FROM_DEFINE)	
		printf(" 使用定义的IP信息配置W5500\r\n");
	// EEPROM方式  --- 实现掉电后参数的保存
#if 0
	/*使用EEPROM存储的IP参数*/	
	if(ip_from==IP_FROM_EEPROM)
	{
    
		/*从EEPROM中读取IP配置信息*/
		read_config_from_eeprom();		
		
		/*如果读取EEPROM中MAC信息,如果已配置，则可使用*/		
		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)		
		{
			printf(" IP from EEPROM\r\n");
			/*复制EEPROM配置信息到配置的结构体变量*/
			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
		}
		else
		{
			printf(" EEPROM未配置,使用定义的IP信息配置W5500,并写入EEPROM\r\n");
			write_config_to_eeprom();	/*使用默认的IP信息，并初始化EEPROM中数据*/
		}			
	}
#endif
	// DHCP方式  ---  用于外网的通信
#if WANeasy
	/*使用DHCP获取IP参数，需调用DHCP子函数*/		
	if(ip_from==IP_FROM_DHCP)								
	{
		printf("使用DHCP方式的IP信息配置W5500\r\n");
		/*复制DHCP获取的配置信息到配置结构体*/
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
			printf(" DHCP子程序未运行,或者不成功\r\n");
		}
	}
#endif

	/*以下配置信息，根据需要选用*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*将IP配置信息写入W5500相应寄存器*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	/* 打印输入的相关的数据 */
	getSIPR (local_ip);			
	printf(" W5500 IP地址   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	printf(" W5500 子网掩码 : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	printf(" W5500 网关     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}
//#if 0
/**
*@brief		配置W5500的MAC地址
*@param		无
*@return	无
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);
	memcpy(DHCP_GET.mac, mac, 6);
}
//#endif


/**
*@brief		配置W5500的GPIO接口
*@param		无
*@return	无
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
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		     /*设置引脚速率为50MHz */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(WIZ_SPIx_SCS_PORT, &GPIO_InitStructure);

	/*定义RESET引脚*/
	GPIO_InitStructure.GPIO_Pin = WIZ_RESET;					       /*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		     /*设置引脚速率为50MHz */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		     /*设置引脚模式为通用推挽输出*/
	GPIO_Init(WIZ_SPIx_RESET_PORT, &GPIO_InitStructure);		 /*调用库函数，初始化GPIO*/
	GPIO_SetBits(WIZ_SPIx_RESET_PORT, WIZ_RESET);
	/*定义INT引脚*/
	GPIO_InitStructure.GPIO_Pin = WIZ_INT;						       /*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		     /*设置引脚速率为100MHz*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		  /*设置引脚模式为通用推挽模拟上拉输入*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(WIZ_SPIx_INT_PORT, &GPIO_InitStructure);			 /*调用库函数，初始化GPIO*/
}


/**
*@brief		W5500片选信号设置函数
*@param		val: 为“0”表示片选端口为低，为“1”表示片选端口为高
*@return	无
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
*@brief		设置W5500的片选端口SCSn为低
*@param		无
*@return	无
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		设置W5500的片选端口SCSn为高
*@param		无
*@return	无
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}

/**
*@brief		W5500复位设置函数
*@param		无
*@return	无
*/
void reset_w5500(void)
{
	GPIO_ResetBits(WIZ_SPIx_RESET_PORT, WIZ_RESET);
	delay_us(2);  
	GPIO_SetBits(WIZ_SPIx_RESET_PORT, WIZ_RESET);
	delay_ms(1600);
}

//#if 0
/* 硬件SPI的写入 */
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
	SPI_I2S_SendData(SPI1, dat);//写1个字节数据
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//等待数据寄存器空
}
#endif
/***************************************************/
/**
*@brief		STM32 SPI1读写8位数据
*@param		dat：写入的8位数据
*@return	无
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_SendByte(dat));
}

/**
*@brief		写入一个8位数据到W5500
*@param		addrbsb: 写入数据的地址
*@param   data：写入的8位数据
*@return	无
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
*@brief		从W5500读出一个8位数据
*@param		addrbsb: 写入数据的地址
*@param   data：从写入的地址处读取到的8位数据
*@return	无
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
*@brief		向W5500写入len字节数据
*@param		addrbsb: 写入数据的地址
*@param   buf：写入字符串
*@param   len：字符串长度
*@return	len：返回字符串长度
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
*@brief		从W5500读出len字节数据
*@param		addrbsb: 读取数据的地址
*@param 	buf：存放读取数据
*@param		len：字符串长度
*@return	len：返回字符串长度
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

/* 设备作为服务端 */
void Server_Task_A(void)
{
	uint16 len = 0;
	switch (getSn_SR(SOCK_TCPS))											            	/*获取socket的状态*/
	{
	case SOCK_CLOSED:													            /*socket处于关闭状态*/
		socket(SOCK_TCPS, Sn_MR_TCP, local_port, Sn_MR_ND);	                        /*打开socket*/
		printf("打开Socket11\r\n");
		break;

	case SOCK_INIT:														                 /*socket已初始化状态*/
		if (listen(SOCK_TCPS))											                /*socket建立监听*/
			printf("Socket11建立监听\r\n");
		else
			printf("Socket11建立监听失败\r\n");
		break;

	case SOCK_ESTABLISHED:		/*socket处于连接建立状态*/
		//printf("识别是否有数据\r\n");
		if (getSn_IR(SOCK_TCPS) & Sn_IR_CON)
		{
			setSn_IR(SOCK_TCPS, Sn_IR_CON);								          /*清除接收中断标志位*/
		}
		len = getSn_RX_RSR(SOCK_TCPS);									            /*定义len为已接收数据的长度*/
		if (len > 0)
		{
			recv(SOCK_TCPS, buff, len);/*接收来自Client的数据*/
			send(SOCK_TCPS, buff, len);/*接收后立即发送数据 */
			buff[len] = 0x00;
			printf("%s\r\n", buff);
			printf("OKA\r\n");
			printf("\r\n");
		}
		break;

	case SOCK_CLOSE_WAIT:												                /*socket处于等待关闭状态*/
		close(SOCK_TCPS);
		printf("Socket11关闭\r\n");
		break;

	}
}

/* 设备作为服务端 */
void Server_Task_B(void)
{
	uint16 len = 0;
	switch (getSn_SR(SOCK_TCPC))											            	/*获取socket的状态*/
	{
	case SOCK_CLOSED:													            /*socket处于关闭状态*/
		socket(SOCK_TCPC, Sn_MR_TCP, local_port, Sn_MR_ND);	                        /*打开socket*/
		printf("打开Socket22\r\n");
		break;

	case SOCK_INIT:														                 /*socket已初始化状态*/
		if (listen(SOCK_TCPC))											                /*socket建立监听*/
			printf("Socket22建立监听\r\n");
		else
			printf("Socket22建立监听失败\r\n");
		break;

	case SOCK_ESTABLISHED:		/*socket处于连接建立状态*/
		//printf("识别是否有数据\r\n");
		if (getSn_IR(SOCK_TCPC) & Sn_IR_CON)
		{
			setSn_IR(SOCK_TCPC, Sn_IR_CON);								          /*清除接收中断标志位*/
		}
		len = getSn_RX_RSR(SOCK_TCPC);									            /*定义len为已接收数据的长度*/
		if (len > 0)
		{
			recv(SOCK_TCPC, buff, len);/*接收来自Client的数据*/
			send(SOCK_TCPC, buff, len);/*接收后立即发送数据 */
			buff[len] = 0x00;
			printf("%s\r\n", buff);
			printf("OKB\r\n");
			printf("\r\n");
		}
		break;

	case SOCK_CLOSE_WAIT:												                /*socket处于等待关闭状态*/
		close(SOCK_TCPC);
		printf("Socket22关闭\r\n");
		break;

	}
}

/*
     测试一下不同的ID端口号   --- 连接设备 测试服务器在不同ID下的数据通信
	 测试一下发送完人为关闭并再连接
*/
/* 设备作为客户端 */
void Client_Task_A(uint8 *sp, uint16 lenth)
{
	uint16 len = 0;
	uint8 sign1 = 0;
	switch (getSn_SR(SOCK_TCPC))								  				         /*获取socket的状态*/
	{
	case SOCK_CLOSED:											        		         /*socket处于关闭状态*/
		//socket(SOCK_TCPC, Sn_MR_TCP, local_port++, Sn_MR_ND);
		local_port=6000;
		socket(SOCK_TCPC, Sn_MR_TCP, local_port, Sn_MR_ND);
		printf("打开socket\r\n");
		break;

	case SOCK_INIT:												/*socket处于初始化状态*/
		connect(SOCK_TCPC, remote_ip, remote_port);                /*socket连接服务器*/
		printf("socket连接服务器\r\n");
		break;

	case SOCK_ESTABLISHED: 												             /*socket处于连接建立状态*/
		if (getSn_IR(SOCK_TCPC) & Sn_IR_CON)
		{
			setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*清除接收中断标志位*/
		}
		len = getSn_RX_RSR(SOCK_TCPC);									            /*定义len为已接收数据的长度*/
		if (len > 0)
		{
			recv(SOCK_TCPC, buff, len);/*接收来自Client的数据*/
			send(SOCK_TCPC, buff, len);/*接收后立即发送数据 */
			buff[len] = 0x00;
			printf("%s\r\n", buff);
			printf("OK2\r\n");
		}
		if (lenth != 0)
		{
			send(SOCK_TCPC, sp, lenth);								     	         /*向Server发送数据*/
			printf("0K3\r\n");

		}
		sign1 = 5;
		delay_ms(1000);
		delay_ms(1000);
		break;
#if 0
	case SOCK_CLOSE_WAIT: 											    	         /*socket处于等待关闭状态*/
		close(SOCK_TCPC);
		printf("socket关闭\r\n");
		break;
#endif 
	}
	if (sign1 == 5)
	{
		close(SOCK_TCPC);
	//	usart1_date = 0;
		date1 = 0;
		printf("socket关闭\r\n");
	}
}

void Client_Task_B(uint8 *sp, uint16 lenth)
{
	uint16 len = 0;
	switch (getSn_SR(SOCK_TCPS))								  				         /*获取socket的状态*/
	{
	case SOCK_CLOSED:											        		         /*socket处于关闭状态*/
		//socket(SOCK_TCPC, Sn_MR_TCP, local_port++, Sn_MR_ND);
		local_port = 6000;
		socket(SOCK_TCPS, Sn_MR_TCP, local_port, Sn_MR_ND);
		printf("打开socket\r\n");
		break;

	case SOCK_INIT:												/*socket处于初始化状态*/
		connect(SOCK_TCPS, remote_ip, remote_port);                /*socket连接服务器*/
		printf("socket连接服务器\r\n");
		break;

	case SOCK_ESTABLISHED: 												             /*socket处于连接建立状态*/
		if (getSn_IR(SOCK_TCPS) & Sn_IR_CON)
		{
			setSn_IR(SOCK_TCPS, Sn_IR_CON); 							         /*清除接收中断标志位*/
		}
		len = getSn_RX_RSR(SOCK_TCPC);									            /*定义len为已接收数据的长度*/
		if (len > 0)
		{
			recv(SOCK_TCPC, buff, len);/*接收来自Client的数据*/
			send(SOCK_TCPC, buff, len);/*接收后立即发送数据 */
			buff[len] = 0x00;
			printf("%s\r\n", buff);
			printf("OK2\r\n");
		}
		if (lenth != 0)
		{
			send(SOCK_TCPS, sp, lenth);								     	         /*向Server发送数据*/
			printf("0K3\r\n");

		}
		delay_ms(1000);
		delay_ms(1000);
		break;

	case SOCK_CLOSE_WAIT: 											    	         /*socket处于等待关闭状态*/
		close(SOCK_TCPS);
		printf("socket关闭\r\n");
		break;

	}
}


#endif
