#include "at32f4xx.h"
#include "at32_board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BSP.h"
#include "mg_api.h"

static u8 advData[] = 
{
    0x02,//Length
    0x01,//Flags
    0x06//BR/EDR Not Supported, LE General Discoverable Mode
};

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    u8 *ble_addr;
    u8 *ble_ver;
    u8 *ble_name;
    u8 *scanRspData;
    u8 scanRspLen;
    
    //UART init
    UART_Print_Init(115200);
    
    //board init
    BSP_Init();
    printf("BSP init\n");
    
    //get firmware version of BLE module
    ble_ver = get_ble_version();
    printf("BLE firmware version %s\n", ble_ver);
    
    //BLE init
    SetBleIntRunningMode();
    radio_initBle(TXPWR_0DBM, &ble_addr);
    printf("BLE init\n");
    printf("BLE device address %02X:%02X:%02X:%02X:%02X:%02X\n",
            ble_addr[5], ble_addr[4], ble_addr[3],
            ble_addr[2], ble_addr[1], ble_addr[0]);
    
    //set advData
    ble_set_adv_data(advData, sizeof(advData));
    
    //set scanRspData
    ble_name = getDeviceInfoData(&scanRspLen);
    scanRspLen += 2;//+1 byte AD Length, +1 byte AD Type
    if(scanRspLen > 31)
    {
        printf("ERROR: scan response data length %d\n", scanRspLen);
        return 0;
    }
    scanRspData = malloc(scanRspLen);
    if(scanRspData == 0)
    {
        printf("NO MEMORY: scanRspData\n");
        return 0;
    }
    scanRspData[0] = scanRspLen - 1;//AD Length
    scanRspData[1] = 0x09;//AD Type: Complete Local Name
    memcpy(scanRspData + 2, ble_name, scanRspLen - 2);
    ble_set_adv_rsp_data(scanRspData, scanRspLen);
    free(scanRspData);
    
    //Disable advertising
    ble_set_adv_enableFlag(0);
    
    //BLE run
    ble_run_interrupt_start(320);//320*0.625=200ms
    printf("BLE run\n");
    
    //Enable advertising
    ble_set_adv_enableFlag(1);
    printf("Advertising start.\n");
    
    //main while
    while(1)
    {
        IrqMcuGotoSleepAndWakeup();
    }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    /* Infinite loop */
    while (1)
    {}
}

#endif
