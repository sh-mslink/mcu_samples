 /***********************************************************************
文件名称：TCP_CLIENT.H
功    能：完成TCP的数据收发
编写时间：2013.4.25
编 写 人：赵
注    意：
***********************************************************************/
#include "stdint.h"
#include "lwip/err.h"

#ifndef  _TCP_CLIENT_H_
#define  _TCP_CLIENT_H_


/***************开发板ip及MAC定义*************************/
#define BOARD_IP  			  192,168,2,5       //client IP
#define BOARD_NETMASK   	255,255,255,0     //client MASK
#define BOARD_GW		   	  192,168,2,1       //client gateway
#define BOARD_MAC_ADDR    0,0,0,3,2,1       //client MAC


#define TCP_LOCAL_PORT     		1030 
#define TCP_SERVER_PORT    		1031

#define TCP_SERVER_IP   192,168,2,19        //server IP

extern struct tcp_pcb *tcp_client_pcb;

void Delay_s(unsigned long ulVal);
void TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length);
struct tcp_pcb *Check_TCP_Connect(void);
#endif

