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
	//tcp_close(cpcb);				//���������ݹر�����,���ݾ������ѡ��ʹ��	
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
	//	if(cpcb->local_port == TCP_LOCAL_PORT && cpcb->remote_port == TCP_SERVER_PORT)		//���TCP_LOCAL_PORT�˿�ָ��������û�жϿ�
		if(cpcb -> state == ESTABLISHED) 
		{
			connect_flag = 1;  						//���ӱ�־
			break;							   	
		}
	}

	if(connect_flag == 0)
	{
		TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP); //��������
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
		tcp_recved(pcb, p->tot_len);				//��ȡ���ݳ��� tot_len��tcp���ݿ�ĳ���
		tcp_write(pcb,p->payload,p->tot_len,TCP_WRITE_FLAG_COPY);
		tcp_output(pcb);			    
	}
	else	 										//����������Ͽ����ӣ���ͻ���ҲӦ�Ͽ�
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
	IP4_ADDR(&ipaddr,a,b,c,d);           //������IP��ַ
	tcp_client_pcb = tcp_new(); /* ����ͨ�ŵ�TCP���ƿ�(Clipcb) */
	if (!tcp_client_pcb)
	{
		return ;
	}	
	err = tcp_bind(tcp_client_pcb,IP_ADDR_ANY,local_port); /* �󶨱���IP��ַ�Ͷ˿ں� ������ip��ַ��LwIP_Init()���Ѿ���ʼ��*/
    if(err != ERR_OK)
	{
		return ;
	}
	tcp_connect(tcp_client_pcb,&ipaddr,remote_port,TCP_Connected);//ע��ص�����
	tcp_recv(tcp_client_pcb,TCP_Client_Recv); 				/* ����tcp���ջص����� */
}


