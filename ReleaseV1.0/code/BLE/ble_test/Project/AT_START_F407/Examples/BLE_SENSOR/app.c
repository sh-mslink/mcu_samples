/*
    Copyright (c) 2017 Macrogiga Electronics Co., Ltd.

    Permission is hereby granted, free of charge, to any person 
    obtaining a copy of this software and associated documentation 
    files (the "Software"), to deal in the Software without 
    restriction, including without limitation the rights to use, copy, 
    modify, merge, publish, distribute, sublicense, and/or sell copies 
    of the Software, and to permit persons to whom the Software is 
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be 
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.
*/

#include <string.h>
#include <stdbool.h>
#include "BSP.h"
#include "mg_api.h"

/// Characteristic Properties Bit
#define ATT_CHAR_PROP_RD            0x02
#define ATT_CHAR_PROP_W_NORSP       0x04
#define ATT_CHAR_PROP_W             0x08
#define ATT_CHAR_PROP_NTF           0x10
#define ATT_CHAR_PROP_IND           0x20
#define GATT_PRIMARY_SERVICE_UUID   0x2800

#define TYPE_CHAR      0x2803
#define TYPE_CFG       0x2902
#define TYPE_INFO      0x2901
#define TYPE_xRpRef    0x2907
#define TYPE_RpRef     0x2908
#define TYPE_INC       0x2802
#define UUID16_FORMAT  0xff

char DeviceInfo[7] = "My Name";//max len is 24 bytes

u16 cur_notifyhandle = 0x05;//Note: make sure each notify handle by invoking function: set_notifyhandle(hd);

char GetConnectedStatus(void);

bool isNtfCfg = false;

extern bool g_Tmr2IntFlag;
extern u8 readHDC2080Data(u8 *pBuffer, u8 size);
extern u8 readHDC2080ID(u8 *pBuffer, u8 size);
extern void calTempAndHumi(u8 *pBuffer, float *pTemp, float *pHumi);

u8* getDeviceInfoData(u8* len)
{
    *len = sizeof(DeviceInfo);
    return (u8*)DeviceInfo;
}

void getTempHumi(float *temp, float *humi)
{
    u8 i2cRxBuf[4];
    float t, h;
    u8 ret;
    
    ret = readHDC2080Data(i2cRxBuf, 4);
    if(ret == 0)
        printf("ERROR: HDC2080 read data error.\n");
    else
    {
        calTempAndHumi(i2cRxBuf, &t, &h);
        printf("T=%.02f H=%.02f\n", t, h);
    }
    
    *temp = t;
    *humi = h;
}

/**********************************************************************************
        *****DataBase****
0x0001  Primary Service         6E400001-B5A3-F393-E0A9-E50E24DCCA9E
0x0002  Characteristic          6E400002-B5A3-F393-E0A9-E50E24DCCA9E
0x0003  Characteristic Value    Write Without Response
0x0004  Characteristic          6E400003-B5A3-F393-E0A9-E50E24DCCA9E
0x0005  Characteristic Value    Notify
0x0006  CCCD
************************************************************************************/

typedef struct ble_character16
{
    u16 type16;          //type2
    u16 handle_rec;      //handle
    u8  characterInfo[5];//property1 - handle2 - uuid2
    u8  uuid128_idx;     //0xff means uuid16, other is idx of uuid128
}
BLE_CHAR;

typedef struct ble_UUID128
{
    u8  uuid128[16];//uuid128 string: little endian
}
BLE_UUID128;

//
///STEP0:Character declare
//
const BLE_CHAR AttCharList[] =
{
    {TYPE_CHAR, 0x0002, {ATT_CHAR_PROP_W_NORSP, 0x03, 0x00, 0, 0}, 1/*uuid128-idx1*/},
    {TYPE_CHAR, 0x0004, {ATT_CHAR_PROP_NTF | ATT_CHAR_PROP_RD, 0x05, 0x00, 0, 0}, 2/*uuid128-idx2*/},
    {TYPE_CFG,  0x0006, {ATT_CHAR_PROP_W | ATT_CHAR_PROP_RD}},//CCCD
};

const BLE_UUID128 AttUuid128List[] =
{
    //idx0,little endian, Service UUID
    {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0,
     0x93, 0xf3, 0xa3, 0xb5, 0x01, 0x00, 0x40, 0x6e},
    
    //idx1,little endian, Char WWR UUID
    {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0,
     0x93, 0xf3, 0xa3, 0xb5, 0xfe, 0x00, 0x40, 0x6e},
    
    //idx2,little endian, Char NTF UUID
    {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0,
     0x93, 0xf3, 0xa3, 0xb5, 0xff, 0x00, 0x40, 0x6e},
};

u8 GetCharListDim(void)
{
    return sizeof(AttCharList)/sizeof(AttCharList[0]);
}

//////////////////////////////////////////////////////////////////////////
///STEP1:Service declare
// read by type request handle, primary service declare implementation
void att_server_rdByGrType(u8 pdu_type, u8 attOpcode, u16 st_hd, u16 end_hd, u16 att_type)
{
    if((att_type == GATT_PRIMARY_SERVICE_UUID) && (st_hd <= 0x0001))
    {
        att_server_rdByGrTypeRspPrimaryService(
                pdu_type, 0x0001, 0x0006, (u8*)(AttUuid128List[0].uuid128), 16/* bytes */);
        return;
    }
    
    //other service added here if any
    //to do....
    
    ///error handle
    att_notFd(pdu_type, attOpcode, st_hd);
}


///STEP2:data coming
///write response, data coming....
void ser_write_rsp(u8 pdu_type/*reserved*/, u8 attOpcode/*reserved*/, 
                   u16 att_hd, u8* attValue/*app data pointer*/, u8 valueLen_w/*app data size*/)
{
    switch(att_hd)
    {
        case 0x0003://write without response
        {
            u8 i;
            printf("Write receive, length %d, data 0x", valueLen_w);
            for(i = 0; i < valueLen_w; i++)
                printf(" %02X", attValue[i]);
            printf("\n");
            break;
        }
        
        case 0x0006://CCCD
        {
            ser_write_rsp_pkt(pdu_type);//response of write
            if((*attValue & 0x0001) != 0)
            {
                isNtfCfg = true;
                printf("Notify enable.\n");
            }
            else if((*attValue & 0x0001) == 0)
            {
                isNtfCfg = false;
                printf("Notify disable.\n");
            }
            break;
        }
        
        default:
            att_notFd(pdu_type, attOpcode, att_hd);/*the default response, also for the purpose of error robust */
            break;
    }
}

///STEP2.1:Queued Writes data if any
void ser_prepare_write(u16 handle, u8* attValue, u16 attValueLen, u16 att_offset)//user's call back api 
{
    //queued data:offset + data(size)
    //when ser_execute_write() is invoked, means end of queue write.
    
    //to do    
}
 
void ser_execute_write(void)//user's call back api
{
    //end of queued writes
    
    //to do...    
}

///STEP3:Read data
//// read response
void server_rd_rsp(u8 attOpcode, u16 attHandle, u8 pdu_type)
{
    switch(attHandle)
    {
        case 0x0005://notify
        {
            u8 attData[4] = {0x00, 0x00};
            float temp, humi;
            u8 i;
            
            //get temp & humi
            getTempHumi(&temp, &humi);
            
            //notify data
            attData[0] = (int)(temp * 100) % 100;
            attData[1] = (char)temp;
            attData[2] = (int)(humi * 100) % 100;
            attData[3] = (char)humi;
            att_server_rd(pdu_type, attOpcode, attHandle, attData, sizeof(attData));
            printf("CCCD read response: 0x");
            for(i = 0; i < sizeof(attData); i++)
                printf(" %02X", attData[i]);
            printf("\n");
            break;
        }
        
        case 0x0006://CCCD
        {
            u8 attData[2] = {0x00, 0x00};
            if(isNtfCfg)
                attData[0] = 0x01;
            att_server_rd(pdu_type, attOpcode, attHandle, attData, 2);
            printf("CCCD read response: 0x%02X%02X.\n", attData[1], attData[0]);
            break;
        }
        
        default:
            att_notFd(pdu_type, attOpcode, attHandle );//the default response, also for the purpose of error robust
            break;
    }
}

void server_blob_rd_rsp(u8 attOpcode, u16 attHandle, u8 dataHdrP,u16 offset)
{
}

//return 1 means found
int GetPrimaryServiceHandle(unsigned short hd_start, unsigned short hd_end,
                            unsigned short uuid16,   
                            unsigned short* hd_start_r,unsigned short* hd_end_r)
{
//example
//    if((uuid16 == 0x1812) && (hd_start <= 0x19))// MUST keep match with the information save in function  att_server_rdByGrType(...)
//    {
//        *hd_start_r = 0x19;
//        *hd_end_r = 0x2a;
//        return 1;
//    }
    
    return 0;
}


//本回调函数可用于蓝牙模块端主动发送数据之用，协议栈会在系统允许的时候（异步）回调本函数，不得阻塞！！
void gatt_user_send_notify_data_callback(void)
{
    u8 ntfData[4];
    u8 sendLength;
    u8 i;
    float temp, humi;
    
    if(g_Tmr2IntFlag)//timer overflow
    {
        //clear flag
        g_Tmr2IntFlag = false;
        
        //get temp & humi
        getTempHumi(&temp, &humi);
        
        //notify data
        if(GetConnectedStatus() && isNtfCfg)
        {
            ntfData[0] = (int)(temp * 100) % 100;
            ntfData[1] = (char)temp;
            ntfData[2] = (int)(humi * 100) % 100;
            ntfData[3] = (char)humi;
            sendLength = sconn_notifydata(ntfData, sizeof(ntfData));
            if(sendLength)
            {
                printf("Notify send, length %d, data 0x", sendLength);
                for(i = 0; i < sendLength; i++)
                    printf(" %02X", ntfData[i]);
                printf("\n");
                
                ntfData[1]++;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
static unsigned char gConnectedFlag = 0;
char GetConnectedStatus(void)
{
    return gConnectedFlag;
}

void ConnectStausUpdate(unsigned char IsConnectedFlag)
{
    u8 *peer_addr;
    u8 peer_addr_type;
    
    isNtfCfg = false;
    
    if(gConnectedFlag != IsConnectedFlag)
    {
        gConnectedFlag = IsConnectedFlag;
        
        if(gConnectedFlag)
        {
            printf("Advertising stop.\n");
            printf("Connected.\n");
            
            peer_addr = GetMasterDeviceMac(&peer_addr_type);
            if(peer_addr_type == 0)
                printf("Peer address type: Public\n");
            else
                printf("Peer address type: Random\n");
            printf("Peer address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                    peer_addr[5], peer_addr[4], peer_addr[3],
                    peer_addr[2], peer_addr[1], peer_addr[0]);
            
            TMR_Cmd(TMR2, ENABLE);//start timer for sensor
        }
        else
        {
            printf("Disconnected.\n");
            printf("Advertising start.\n");
            
            TMR_Cmd(TMR2, DISABLE);//stop timer of sensor
        }
    }
}

unsigned char aes_encrypt_HW(unsigned char *_data, unsigned char *_key)
{
    return 0;//HW AES NOT supported
}
