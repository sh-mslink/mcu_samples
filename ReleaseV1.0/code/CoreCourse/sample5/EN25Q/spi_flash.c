/**
  ******************************************************************************
  * @file    SPI/SPI_W25Q_Flash/spi_flash.c 
  * @author  ArteryTek Technology
  * @version V1.0.0
  * @date    2019-12-25
  * @brief   W25Q flash source code
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

#include "at32f4xx.h"
#include "spi_flash.h"

/* Define External SPI flash unlock key --------------------------------------*/
#define FLASH_KEY1                      ((UINT32)0x45670123)
#define FLASH_KEY2                      ((UINT32)0xCDEF89AB)

uint8_t SpiFlash_SectorBuf[SPIF_SECTOR_SIZE];/* a sector size */	

uint8_t SpiFlash_Init(void)
{
  GPIO_InitType GPIO_InitStructure;
  SPI_InitType  SPI_InitStructure;

  RCC_APB2PeriphClockCmd(SPIx_RCC_CLK | SPIx_GPIO_RCC_CLK, ENABLE);
  RCC_AHBPeriphClockCmd(SPIx_DMA_RCC_CLK, ENABLE);

  /* Configure SPI_FLASH pins*/
  GPIO_InitStructure.GPIO_Pins =  SPIx_PIN_MOSI;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPIx_PORT_MOSI, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pins = SPIx_PIN_MISO;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPIx_PORT_MISO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pins = SPIx_PIN_NSS;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
  GPIO_Init(SPIx_PORT_NSS, &GPIO_InitStructure);  

  GPIO_InitStructure.GPIO_Pins =  SPIx_PIN_SCK;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPIx_PORT_SCK, &GPIO_InitStructure);

  FLASH_CS_HIGH();
  /* SPI_FLASH configuration ------------------------------------------------------*/
  SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_FULLDUPLEX;  //ȫ˫��ģʽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2EDGE;   //�ڶ���ʱ�ӱ��ز�������
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;  //���߿���״̬ʱʱ����Ϊ��
  SPI_InitStructure.SPI_CPOLY = 0;      //ָ������CRCУ��Ķ���ʽ�����ﲻ������
  SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;  //�����շ���λ��ǰ
  SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;  //�������ֽ�(8bit)Ϊ��λ
  SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_32;  //SPI ʱ��32��Ƶ
  SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT; //�������CS�ܽ�
  SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER; //��Ƭ��ʱ����ģʽ
  SPI_Init(FLASH_SPI, &SPI_InitStructure);

  /* Enable SPI module */
  SPI_Enable(FLASH_SPI, ENABLE);

}

/**
  * @brief  Write data to flash
  * @param  pbBuffer: buffer name
  * @param  dwWriteAddr: buffer address 
  * @param  dwNumByteToWrite: buffer length 
  * @retval none
  */
void SpiFlash_Write(uint8_t* pbBuffer, uint32_t dwWriteAddr, uint32_t dwNumByteToWrite)   
{ 
  uint32_t dwSectorPos;
  uint16_t wSectorOffset;
  uint16_t wSectorRemain;	   
 	uint16_t i;    
  uint8_t * SpiFlash_BUF;	  
  SpiFlash_BUF = SpiFlash_SectorBuf;	     
 	dwSectorPos = dwWriteAddr / SPIF_SECTOR_SIZE; // sector address  
  wSectorOffset = dwWriteAddr % SPIF_SECTOR_SIZE; // address offset in a sector
  wSectorRemain = SPIF_SECTOR_SIZE - wSectorOffset; // the remain in a sector 
 	if(dwNumByteToWrite <= wSectorRemain) //ȷ��Ҫд������ݴ�С��sector��������
  {
    wSectorRemain = dwNumByteToWrite; // smaller than a sector size  //���������������ô�ʹ���
  }
  while(1) 
  {	
		//Ϊ�˿ɿ����������κ�д�����ǰ��Ҫ�Ը����������жϣ������ǲ�����ı�������
		//Flash�����������е��ֽڶ���0xFF
    SpiFlash_Read(SpiFlash_BUF, dwSectorPos * SPIF_SECTOR_SIZE, SPIF_SECTOR_SIZE); // read a sector
    for(i=0; i<wSectorRemain; i++) 
    {
      if(SpiFlash_BUF[wSectorOffset + i]!=0xFF)
      {
         break; //t here are some data not equal 0xFF, so this secotr needs erased
      }
    }
		//���û�б���������ô��Ҫ�Ȳ���������
    if(i < wSectorRemain) // need erase
    {
      SpiFlash_Erase_Sector(dwSectorPos); // erase the sector
      for(i=0; i<wSectorRemain; i++) // copy the write data
      {
        SpiFlash_BUF[i+wSectorOffset] = pbBuffer[i];	  
      }
      SpiFlash_Write_NoCheck(SpiFlash_BUF, dwSectorPos * SPIF_SECTOR_SIZE, SPIF_SECTOR_SIZE); // program the sector
    }
    else 
    {
      SpiFlash_Write_NoCheck(pbBuffer, dwWriteAddr, wSectorRemain); // write directly in the erased area                  
    }
    if(dwNumByteToWrite == wSectorRemain)
    {
      break; // write end
    }
		//���Ҫд������ݲ���sector��С������������ô��Ҫ������д
    else // go on writing
    {
      dwSectorPos++;
      wSectorOffset = 0;	 

      pbBuffer += wSectorRemain;  				
      dwWriteAddr += wSectorRemain;				   
      dwNumByteToWrite -= wSectorRemain;			
      if(dwNumByteToWrite > SPIF_SECTOR_SIZE)
      {
        wSectorRemain = SPIF_SECTOR_SIZE; // Could not write the remain data in the next sector
      }
      else
      {
        wSectorRemain = dwNumByteToWrite; // Could write the remain data in the next sector
      }
    }	 
  }
}

/**
  * @brief  Read data from flash
  * @param  pbBuffer: buffer name
  * @param  dwReadAddr: buffer address
  * @param  dwNumByteToRead: buffer length
  * @retval none
  */
void SpiFlash_Read(uint8_t* pbBuffer, uint32_t dwReadAddr, uint32_t dwNumByteToRead)
{
  FLASH_CS_LOW();  //����Ƭѡ
  SPI_WriteByte(SPIF_ReadData); // ���Ͷ�����ָ��
  SPI_WriteByte((uint8_t)((dwReadAddr) >> 16)); //����24bit��ַ�е�����ֽ�
  SPI_WriteByte((uint8_t)((dwReadAddr) >> 8)); //����24bit��ַ�е��м��ֽ�
  SPI_WriteByte((uint8_t)dwReadAddr); //����24bit��ַ�еĵ��ֽ�
  SPI_ReadBytes(pbBuffer,dwNumByteToRead); //��ȡ����
  FLASH_CS_HIGH(); //����Ƭѡ
}

/**
  * @brief  Erase a sector data
  * @param  dwDstAddr: Sector address to erase
  * @retval none
  */
void SpiFlash_Erase_Sector(uint32_t dwDstAddr)
{
 	dwDstAddr*=SPIF_SECTOR_SIZE; // translate sector address to byte address
  SpiFlash_Write_Enable();   //����ָ��ʹ�ܲ�������
  SpiFlash_Wait_Busy();  //��ȡFlash�е�״̬�Ĵ�����ֱ��Flash��������
  FLASH_CS_LOW(); //����CS
  SPI_WriteByte(SPIF_SectorErase); //���Ͳ���ָ��
  SPI_WriteByte((uint8_t)((dwDstAddr) >> 16));  //���Ͳ��������ĵ�ַ
  SPI_WriteByte((uint8_t)((dwDstAddr) >> 8));
  SPI_WriteByte((uint8_t)dwDstAddr);
  FLASH_CS_HIGH(); //����CS
  SpiFlash_Wait_Busy(); //Flash�Ĳ�����д������ʱ���Ƚϳ���һ��Ҫ������mS������ڵ�Ƭ�����Էǳ����ã����Ҫ�ȴ�Flash�������
}

/**
  * @brief  Write data without check 
  * @param  pbBuffer: buffer name
  * @param  dwWriteAddr: buffer address 
  * @param  dwNumByteToWrite: buffer length 
  * @retval none
  */
void SpiFlash_Write_NoCheck(uint8_t* pbBuffer, uint32_t dwWriteAddr, uint32_t dwNumByteToWrite)   
{
  uint16_t wPageRemain;
  wPageRemain=SPIF_PAGE_SIZE - dwWriteAddr % SPIF_PAGE_SIZE; // remain bytes in a page
  if(dwNumByteToWrite<=wPageRemain)
  {
    wPageRemain=dwNumByteToWrite;// smaller than a page size
  }
	while(1)
	{
    SpiFlash_Write_Page(pbBuffer, dwWriteAddr, wPageRemain);
		if(dwNumByteToWrite==wPageRemain)
    {
      break; // all data are programmed 
    }
	 	else // NumByteToWrite>pageremain
		{
      pbBuffer += wPageRemain;
      dwWriteAddr += wPageRemain;
      dwNumByteToWrite -= wPageRemain; // the remain bytes to be prorammed
      if(dwNumByteToWrite > SPIF_PAGE_SIZE)
      {
        wPageRemain = SPIF_PAGE_SIZE; // can be progrmmed a page at a time
      }
      else
      {
        wPageRemain = dwNumByteToWrite; // smaller than a page size
      }
		}
	}
}

/**
  * @brief  Write a page data
  * @param  pbBuffer: buffer name
  * @param  dwWriteAddr: buffer address 
  * @param  dwNumByteToWrite: buffer length
  * @retval none
  */
void SpiFlash_Write_Page(uint8_t* pbBuffer, uint32_t dwWriteAddr, uint32_t dwNumByteToWrite)
{
  if((0 < dwNumByteToWrite) && (dwNumByteToWrite <= SPIF_PAGE_SIZE))
  {
    SpiFlash_Write_Enable(); // SET WEL
    FLASH_CS_LOW();
    SPI_WriteByte(SPIF_PageProgram); // send instruction
    SPI_WriteByte((uint8_t)((dwWriteAddr) >> 16)); // send 24-bit address
    SPI_WriteByte((uint8_t)((dwWriteAddr) >> 8));
    SPI_WriteByte((uint8_t)dwWriteAddr);
    SPI_WriteBytes(pbBuffer,dwNumByteToWrite);
    FLASH_CS_HIGH();
    SpiFlash_Wait_Busy(); // Wait for program end
  }
}

/**
  * @brief  Write data continuously
  * @param  pbBuffer: buffer name
  * @param  dwNumByteToWrite: buffer length
  * @retval none
  */
static void SPI_WriteBytes(uint8_t* pbBuffer, uint32_t dwNumByteToWrite)
{
  volatile uint8_t dummy_data;

#if (SPI_MODE == 0)
  DMA_InitType DMA_InitStructure;

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&FLASH_SPI->DT);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&dummy_data;
  DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;
  DMA_InitStructure.DMA_BufferSize = dwNumByteToWrite;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
  DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_DISABLE;
  DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
  DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
  DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
  DMA_Init(SPIx_Rx_DMA_Channel, &DMA_InitStructure);

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&FLASH_SPI->DT);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pbBuffer;
  DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;
  DMA_InitStructure.DMA_BufferSize = dwNumByteToWrite;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
  DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
  DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
  DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
  DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
  DMA_Init(SPIx_Tx_DMA_Channel, &DMA_InitStructure);

  SPI_I2S_DMAEnable(FLASH_SPI, SPI_I2S_DMA_TX,ENABLE);
  SPI_I2S_DMAEnable(FLASH_SPI, SPI_I2S_DMA_RX,ENABLE);

  DMA_ChannelEnable(SPIx_Rx_DMA_Channel, ENABLE);
  DMA_ChannelEnable(SPIx_Tx_DMA_Channel, ENABLE);

  while(DMA_GetFlagStatus(SPIx_Rx_DMA_FLAG) != SET);
  DMA_ClearFlag(SPIx_Rx_DMA_FLAG);

  DMA_ChannelEnable(SPIx_Tx_DMA_Channel, DISABLE);
  DMA_ChannelEnable(SPIx_Rx_DMA_Channel, DISABLE);
  SPI_I2S_DMAEnable(FLASH_SPI, SPI_I2S_DMA_RX, DISABLE);
  SPI_I2S_DMAEnable(FLASH_SPI, SPI_I2S_DMA_TX, DISABLE);
#else

  while(dwNumByteToWrite--)
  {
    while(SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TE) == RESET);
    SPI_I2S_TxData(FLASH_SPI, *pbBuffer);
    while(SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RNE) == RESET);
    dummy_data = SPI_I2S_RxData(FLASH_SPI);
    pbBuffer ++;
  }
#endif
}

/**
  * @brief  Wait program done
  * @param  none
  * @retval none
  */
static void SpiFlash_Wait_Busy(void)
{
	while((SpiFlash_ReadSR1()&0x01) == 0x01);
} 

/**
  * @brief  Read SR1 register
  * @param  none
  * @retval none
  */
static uint8_t SpiFlash_ReadSR1(void)
{
  uint8_t bReadByte=0;
  FLASH_CS_LOW();
  SPI_WriteByte(SPIF_ReadStatusReg1);
  bReadByte=(uint8_t)SPI_ReadByte();
  FLASH_CS_HIGH();
	return (bReadByte);
}

/**
  * @brief  Enable write operation
  * @param  none
  * @retval none
  */
static void SpiFlash_Write_Enable(void)   
{
  FLASH_CS_LOW();
  SPI_WriteByte(SPIF_WriteEnable);
  FLASH_CS_HIGH();
}

/**
  * @brief  Read device ID
  * @param  none
  * @retval device ID
  */
uint16_t SpiFlash_ReadID(void)
{
	uint16_t wReceiveData = 0;
  FLASH_CS_LOW(); //����Ƭѡ
  SPI_WriteByte(SPIF_ManufactDeviceID); //���Ͷ�ȡIDָ��
  SPI_WriteByte(0x00);  //����Flash�����ֲ�˵��������dummy����
  SPI_WriteByte(0x00);  //����Flash�����ֲ�˵��������dummy����
  SPI_WriteByte(0x00);  //���Ͷ�ȡID���ݵ�ģʽ
  wReceiveData|=SPI_ReadByte() << 8; //��ȡ���ֽ�����
  wReceiveData|=SPI_ReadByte();  //��ȡ���ֽ�����
  FLASH_CS_HIGH(); //����Ƭѡ
	return wReceiveData;
}  

/**
  * @brief  Write a byte to flash
  * @param  bWriteValue: Data to write
  * @retval Flash return data
  */
static uint8_t SPI_WriteByte(uint8_t bWriteValue)
{
  uint8_t bRxBuff;
  SPI_I2S_DMAEnable(FLASH_SPI, SPI_I2S_DMA_TX, DISABLE);
  SPI_I2S_DMAEnable(FLASH_SPI, SPI_I2S_DMA_RX, DISABLE);
  SPI_I2S_TxData(FLASH_SPI, bWriteValue);
  while(!SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RNE));
  bRxBuff = SPI_I2S_RxData(FLASH_SPI);
  while(SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_BUSY)); 
  return bRxBuff;
}
 
/**
  * @brief  Read a byte to flash
  * @param  none
  * @retval Flash return data
  */ 
static uint8_t SPI_ReadByte(void)
{
  return (SPI_WriteByte(FLASH_SPI_DUMMY_BYTE));
}

/**
  * @brief  Read data continuously
  * @param  pbBuffer: Buffer to save data
  * @param  dwNumByteToRead: Buffer length
  * @retval none
  */
static void SPI_ReadBytes(uint8_t *pbBuffer, uint32_t dwNumByteToRead)
{
  uint8_t bWriteValue = FLASH_SPI_DUMMY_BYTE;

#if (SPI_MODE == 0)
  DMA_InitType DMA_InitStructure;

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&FLASH_SPI->DT);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&bWriteValue;
  DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;
  DMA_InitStructure.DMA_BufferSize = dwNumByteToRead;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
  DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_DISABLE;
  DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
  DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
  DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
  DMA_Init(SPIx_Tx_DMA_Channel, &DMA_InitStructure);

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&FLASH_SPI->DT);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pbBuffer;
  DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;
  DMA_InitStructure.DMA_BufferSize = dwNumByteToRead;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
  DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
  DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
  DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
  DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
  DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE; 
  DMA_Init(SPIx_Rx_DMA_Channel, &DMA_InitStructure); 

  SPI_I2S_DMAEnable(FLASH_SPI,SPI_I2S_DMA_TX, ENABLE);
  SPI_I2S_DMAEnable(FLASH_SPI,SPI_I2S_DMA_RX, ENABLE);    
  DMA_ChannelEnable(SPIx_Rx_DMA_Channel, ENABLE);
  DMA_ChannelEnable(SPIx_Tx_DMA_Channel, ENABLE);

  while(DMA_GetFlagStatus(SPIx_Rx_DMA_FLAG)!=SET);
  DMA_ClearFlag(SPIx_Rx_DMA_FLAG);

  DMA_ChannelEnable(SPIx_Tx_DMA_Channel, DISABLE);
  DMA_ChannelEnable(SPIx_Rx_DMA_Channel, DISABLE);
  SPI_I2S_DMAEnable(FLASH_SPI, SPI_I2S_DMA_TX, DISABLE);
  SPI_I2S_DMAEnable(FLASH_SPI, SPI_I2S_DMA_RX, DISABLE);
#else

  while(dwNumByteToRead--)
  {
    while(SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TE) == RESET);
    SPI_I2S_TxData(FLASH_SPI, bWriteValue);
    while(SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RNE) == RESET);
    *pbBuffer = SPI_I2S_RxData(FLASH_SPI);
    pbBuffer++;
  }
#endif /* SPI_MODE */
}

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
