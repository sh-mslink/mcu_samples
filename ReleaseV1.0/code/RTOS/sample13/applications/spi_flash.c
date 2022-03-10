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

#include "drv_spi.h"
#include "drv_gpio.h"
#include <rtdevice.h>ma
#include <rtthread.h>
#include <finsh.h>
#include <dfs_fs.h> 
#include "spi_flash.h"
#include "spi_flash_sfud.h"


#define FLASH_SPI_DEVICE_NAME     "spi20"
#define FLASH_BUS_NAME     "spi2"
struct rt_spi_device *spi_dev_flash;
uint8_t SpiFlash_SectorBuf[SPIF_SECTOR_SIZE];/* a sector size */	

static int flash_spi_device_init(void)
{
	if (rt_hw_spi_device_attach(FLASH_BUS_NAME, FLASH_SPI_DEVICE_NAME, SPIx_PORT_NSS, SPIx_PIN_NSS)!= RT_EOK)
	{
		rt_kprintf("spi2 device attach error~!\r\n");
		return RT_ERROR;
	}
	else 
	{
		rt_kprintf("spi2 device attach success~!\r\n");
	}
 
	/* ?? SFUD ?? spi20 ???,?? spi20 ??? flash ???????,?? W25Q128 */
	if (RT_NULL == rt_sfud_flash_probe("EN25Q128", "spi20"))
	{
		return RT_ERROR;
	}
	return RT_EOK;
}
INIT_DEVICE_EXPORT(flash_spi_device_init);


void file_system_mount(void)
{
	if(dfs_mount("EN25Q128", "/", "elm", 0, 0) == RT_NULL)
	{
		rt_kprintf("spi flash mount to /!\n");
	}
	else
	{
		rt_kprintf("mount failed, and trying to make FS now...\n");
		dfs_mkfs("elm", "EN25Q128");
		if(dfs_mount("EN25Q128", "/", "elm", 0, 0) != RT_NULL)
			rt_kprintf("spi flash mount to / failed!\n");
		else
			rt_kprintf("spi flash mount to /!\n");
	}
}


#if 0
static void spi_en25q_sample(int argc, char *argv[])
{
    struct rt_spi_device *spi_dev_en25q;
    char name[RT_NAME_MAX];
    rt_uint8_t en25x_read_id[4] = {0x90,0x00,0x00,0x00};
    rt_uint8_t id[2] = {0};
    rt_uint8_t id1[2] = {0};

    if (argc == 2)
    {
        rt_strncpy(name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(name, FLASH_SPI_DEVICE_NAME, RT_NAME_MAX);
    }


    /* ?? spi ???????? */
    spi_dev_en25q = (struct rt_spi_device *)rt_device_find(name);
    if (!spi_dev_en25q)
    {
        rt_kprintf("spi sample run failed! can't find %s device!\n", name);
    }
    else
    {
        /* config spi */
        {
            struct rt_spi_configuration cfg;
            cfg.data_width = 8;
            cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB; /* SPI Compatible: Mode 0 and Mode 3 */
            cfg.max_hz = 50 * 1000 * 1000; /* 50M */
            rt_spi_configure(spi_dev_en25q, &cfg);
        }

        /* ??1:?? rt_spi_send_then_recv()??????ID */
        rt_spi_send_then_recv(spi_dev_en25q, en25x_read_id, 4, id, 2);
        rt_kprintf("use rt_spi_send_then_recv() read en2s5q ID is:%x%x\n", id[0], id[1]);

                struct rt_spi_message msg1, msg2;

                msg1.send_buf   = en25x_read_id;
                msg1.recv_buf   = RT_NULL;
                msg1.length     = 4;
                msg1.cs_take    = 1;
                msg1.cs_release = 0;
                msg1.next       = &msg2;

                msg2.send_buf   = RT_NULL;
                msg2.recv_buf   = id1;
                msg2.length     = 2;
                msg2.cs_take    = 0;
                msg2.cs_release = 1;
                msg2.next       = RT_NULL;

                rt_spi_transfer_message(spi_dev_en25q, &msg1);
                rt_kprintf("use rt_spi_transfer_message() read en25q ID is:%x%x\n", id1[0], id1[1]);

    }
}
/* ??? msh ????? */
MSH_CMD_EXPORT(spi_en25q_sample, spi en25q sample);
#endif
/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
