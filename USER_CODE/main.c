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

typedef union
{
    uint32_t i;
    float f;
}value;
float temp;
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
    value humi_val,temp_val;
    char error,checksum;
    uint32_t i;
    SystemInit();                                                       /* ϵͳ��ʼ��������ɾ��         */
    GPIOInit();
    myDelay(20);                                                        //�ϵ�֮����Ҫ�ȴ�11ms��Խ�������ߡ�״̬
    while (1) 
    {
        error=0;
        temp_val.i=0;
        error += s_measure((char*) &(temp_val.i),&checksum,TEMP);
        temp_val.f = (float)(temp_val.i);
        temp = temp_val.f;
        myDelay(500);
    }
}

/*********************************************************************************************************
End Of File
*********************************************************************************************************/
