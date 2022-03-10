/**
 ******************************************************************************
 * @file        ETH/tcp_client/tcp_client.c
 * @brief       implement tcp layer function
 * @author      Artery Technology
 * @date        2019-01-04
 * @version     V1.1.2
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, ARTERYTEK SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2018 ArteryTek</center></h2>
 ******************************************************************************
 */
#include "main.h"

/* Gloable functions ---------------------------------------------------------*/
unsigned char connect_flag = 0;

/**
  * @brief  Generate delay
  * @param  loop times
  * @retval None
  */
void Delay_s(unsigned long ulVal)
{
	while ( --ulVal != 0 );
}

/**
  * @brief  TCP client send data
  * @param  cpcb
  * @param  buff
  * @param  length
  * @retval None
  */
err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length)
{
	err_t err;

	err = tcp_write(cpcb,buff,length,TCP_WRITE_FLAG_COPY);	//send data
	tcp_output(cpcb);
	//tcp_close(cpcb);				//发送完数据关闭连接,根据具体情况选择使用	
	return err;					
}

/**
  * @brief  check connection
  * @param  None
  * @retval cpcb
  */
struct tcp_pcb *Check_TCP_Connect(void)
{
	struct tcp_pcb *cpcb = 0;
	connect_flag = 0;
	for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next)
	{
	//	if(cpcb->local_port == TCP_LOCAL_PORT && cpcb->remote_port == TCP_SERVER_PORT)		//如果TCP_LOCAL_PORT端口指定的连接没有断开
		if(cpcb -> state == ESTABLISHED) 
		{
			connect_flag = 1;  						//连接标志
			break;							   	
		}
	}

	if(connect_flag == 0)
	{
		TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP); //重新连接
		cpcb = 0;
	}
	return cpcb;	
}

/**
  * @brief  report connection is establish
  * @param  arg
  * @param  pcb
  * @param  err
  * @retval ERR_OK
  */
err_t TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	//tcp_client_pcb = pcb;
	return ERR_OK;
}

/**
  * @brief  receiving callback funtion for TCP client
  * @param  arg
  * @param  pcb
  * @param  pbuf
  * @param  err
  * @retval err
  */
err_t TCP_Client_Recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{

	if(p != NULL)
	{
		tcp_recved(pcb, p->tot_len);				//获取数据长度 tot_len：tcp数据块的长度
		tcp_write(pcb,p->payload,p->tot_len,TCP_WRITE_FLAG_COPY);
		tcp_output(pcb);			    
	}
	else	 										//如果服务器断开连接，则客户端也应断开
	{
		tcp_close(pcb); 						
	}
	pbuf_free(p); 				
	err = ERR_OK;
	return err;	
}

/**
  * @brief  Initialize TCP client
  * @param  local_port
  * @param  remote_port
  * @param  a
  * @param  b
  * @param  c
  * @param  d
  * @retval None
  */
void TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{

	struct ip_addr ipaddr;
	struct tcp_pcb *tcp_client_pcb;
	err_t err;
	IP4_ADDR(&ipaddr,a,b,c,d);           //服务器IP地址
	tcp_client_pcb = tcp_new(); /* 建立通信的TCP控制块(Clipcb) */
	if (!tcp_client_pcb)
	{
		return ;
	}	
	err = tcp_bind(tcp_client_pcb,IP_ADDR_ANY,local_port); /* 绑定本地IP地址和端口号 ，本地ip地址在LwIP_Init()中已经初始化*/
    if(err != ERR_OK)
	{
		return ;
	}
	tcp_connect(tcp_client_pcb,&ipaddr,remote_port,TCP_Connected);//注册回调函数
	tcp_recv(tcp_client_pcb,TCP_Client_Recv); 				/* 设置tcp接收回调函数 */
}


