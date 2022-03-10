#include "BSP.h"
#include "mg_api.h"


#define MAX_SIZE 200
u8 txBuf[MAX_SIZE], rxBuf[MAX_SIZE];
static u16 RxCont = 0;
static u8 PosW = 0, txLen = 0;

extern char GetConnectedStatus(void);


void USART2_IRQHandler(void)
{
	
    if(USART_GetITStatus(USART2, USART_INT_RDNE) != RESET)
    {
        rxBuf[RxCont] = USART_ReceiveData(USART2);
        RxCont++;
        if(RxCont >= MAX_SIZE)
        {
            RxCont = 0;
        }
    }
  
    if(USART_GetITStatus(USART2, USART_INT_TDE) != RESET)
    {   
        if (PosW < txLen)
        {
            USART_SendData(USART2, txBuf[PosW++]);
        }
        
        if (PosW >= txLen){
            txLen = 0;
            PosW = 0;
            USART_INTConfig(USART2, USART_INT_TDE, DISABLE);
        }  
    }
}


void moduleOutData(u8*data, u8 len) //api
{
    unsigned char i;

    if ((txLen+len)<MAX_SIZE)//buff not overflow
    {
        for (i=0;i<len;i++)
        {
            txBuf[txLen+i] = *(data+i);
        }
        txLen += len;
    }
}

#define comrxbuf_wr_pos RxCont
u16 comrxbuf_rd_pos = 0; //init, com rx buffer
void CheckComPortInData(void) //at cmd NOT supported
{
    u16 send;
    
    if(comrxbuf_wr_pos != comrxbuf_rd_pos)//not empty
    {
        if(!GetConnectedStatus())
        {
            comrxbuf_rd_pos = comrxbuf_wr_pos; //empty the buffer if any
        }
        else //connected
        {
            if(comrxbuf_wr_pos > comrxbuf_rd_pos)
            {
                send = sconn_notifydata(rxBuf+comrxbuf_rd_pos,comrxbuf_wr_pos - comrxbuf_rd_pos);
                comrxbuf_rd_pos += send;
            }
            else 
            {
                send = sconn_notifydata(rxBuf+comrxbuf_rd_pos,MAX_SIZE - comrxbuf_rd_pos);
                comrxbuf_rd_pos += send;
                comrxbuf_rd_pos %= MAX_SIZE;
            }
        }
    }
}

void UsrProcCallback(void) //porting api
{
    static u8 led_flash = 0;
    
    IWDG_ReloadCounter();
    
    led_flash ++;
    LED_ONOFF(led_flash & 0x1);
    
    CheckComPortInData();
    if ((txLen) && (0 == PosW))
    {
        USART_SendData(USART2, txBuf[PosW++]);
        USART_INTConfig(USART2, USART_INT_TDE, ENABLE);
    }
}
