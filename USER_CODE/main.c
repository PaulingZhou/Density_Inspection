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
** Descriptions:        ����Ũ�ȼ�
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
#include "LPC11xx.h"                                                    /* LPC11xx����Ĵ���            */
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
** Descriptions:        ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int main (void)
{
    uint8_t a = 0x0A;
    value humi_val,temp_val;
    char error,checksum;
//     uint32_t i;
    SystemInit();                                                       /* ϵͳ��ʼ��������ɾ��         */
    GPIOInit();
    uartInit();
    myDelay(20);                                                        //�ϵ�֮����Ҫ�ȴ�11ms��Խ�������ߡ�״̬
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
