/****************************************Copyright (c)****************************************************
**                                          
**
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2015-3-15
** Last Version:        V1.0
** Descriptions:        The main() function 
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Zhou Sihang
** Created date:        2015-3-15
** Version:             V1.00
** Descriptions:        气体浓度计
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**
** Rechecked by:
*********************************************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx外设寄存器            */
#include "SHT10.h"
#include "init.h"
#include "uart.h"

typedef union
{
    uint32_t i;
    float f;
}value;
float temp,humi;
// uint8_t val_1,val_2;

/*********************************************************************************************************
** Function name:       main
** Descriptions:        无
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main (void)
{
    uint8_t a = 0x0A;
    value humi_val,temp_val;
    char error,checksum;
//     uint32_t i;
    SystemInit();                                                       /* 系统初始化，切勿删除         */
    GPIOInit();
    uartInit();
    myDelay(20);                                                        //上电之后需要等待11ms以越过“休眠”状态
    while (1) 
    {
//         error=0;
//         temp_val.i=0;
//         humi_val.i = 0;
//         error += s_measure((char*) &(temp_val.i),&checksum,TEMP);
//         error += s_measure((char*)&(humi_val.i),&checksum,HUMI);
//         temp_val.f = (float)(temp_val.i);
//         humi_val.f = (float)(humi_val.i);
//         temp = -40+0.01*temp_val.f;
//         humi = -4+0.0405*humi_val.f-2.8/1000000*humi_val.f*humi_val.f;
//         myDelay(1000);
        uartSendByte(a);
        myDelay(500);
    }
}

/*********************************************************************************************************
End Of File
*********************************************************************************************************/
