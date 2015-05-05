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
#include "ds18b20.h"

typedef union
{
    uint32_t i;
    float f;
}value;
float temp,humi;
value humi_val,temp_val;
uint32_t length;
volatile uint32_t   GuiCapFlag = 0;                                     /* ��ʱ�������жϱ�־           */
uint32_t i,tmr0_opcnt=0;
// uint8_t val_1,val_2;

/*********************************************************************************************************
* Function Name:        TIMER16_0_IRQHandler
* Description:          TIMER16_0 �жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void TIMER32_1_IRQHandler (void)
{
    static uint8_t i;
    if (i == 1) {
        i = 0;
        LPC_TMR32B1->IR  = 1 << 4;                                      /* ���CAP0.0�жϱ�־           */
        GuiCapFlag = 1;
    }
    i++;
//     myDelay(50);
}

/*********************************************************************************************************
* Function Name:        TIMER16_0_IRQHandler
* Description:          TIMER16_0 �жϴ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void TIMER16_0_IRQHandler (void)
{
    LPC_TMR16B0->IR = 0x01;                                             /* ����жϱ�־                 */
    tmr0_opcnt++;
    if(tmr0_opcnt == 10){
        LPC_TMR16B0->TCR = 0x00;
        tmr0_opcnt = 0;        
    }
}

/*********************************************************************************************************
** Function name:       main
** Descriptions:        ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int main (void)
{
    uint8_t val[4];
    char error,checksum;

    SystemInit();                                                       /* ϵͳ��ʼ��������ɾ��         */
    GPIOInit();
    uartInit();
    timer0Init();
    timer1Init();
    myDelay(20);                                                        //�ϵ�֮����Ҫ�ȴ�11ms��Խ�������ߡ�״̬
    while (1) 
    {
        ds_write_byte(0x00);
//         
//         
//         DS18B20_Reset();
//         myDelay(2);
//         
//         
//         LPC_TMR16B0->TCR = 0x01;
//         myDelay(1);
//         if(GuiCapFlag){
//         length = LPC_TMR32B1->CR0; 
//         GuiCapFlag  = 0;
//         }
//         temp_val.i=0;
//         humi_val.i = 0;
//         error=0;    
//         error += s_measure((char*) &(temp_val.i),&checksum,TEMP);
//         error += s_measure((char*)&(humi_val.i),&checksum,HUMI);
//         uartSendByte(0xAA);
//         uartSendByte(0xBB);
//         for(i=0;i<2;i++)
//         {
//             val[i] = *((char*)(&temp_val.i)+i);
//             uartSendByte(val[i]);
//         }
//         for(i=2;i<4;i++)
//         {
//             val[i] = *((char*)(&humi_val.i)+(i-2));
//             uartSendByte(val[i]);
//         }
// //         temp_val.f = (float)(temp_val.i);
// //         humi_val.f = (float)(humi_val.i);
// //         temp = -40+0.01*temp_val.f;
// //         humi = -4+0.0405*humi_val.f-2.8/1000000*humi_val.f*humi_val.f;
//         myDelay(1000);
    }
}

/*********************************************************************************************************
End Of File
*********************************************************************************************************/
