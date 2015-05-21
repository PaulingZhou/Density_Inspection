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

#define lengthUart 0x07                                //�궨�� ����֡���� ����֡ͷ����������λ�ĳ��ȣ���Ϊ����(1byte)��������(1byte)��
                                                       //�¶ȣ�ʪ�ȣ���ʱ(��2byte)��У��(1bit)
#define cmdUartSend 0x07                               //�궨�� ������ ��0λ��ʾ�¶ȣ���1λ��ʾʪ�ȣ���2λ��ʶ��ʱ����1��ʾ����

typedef union
{
    uint32_t i;
    float f;
}value;
float temp,humi;
value humi_val,temp_val;
uint32_t timerCount = 0;
volatile uint8_t GuiCapFlag = 0;                                     /* ��ʱ�������жϱ�־           */
uint32_t i,tmr0_opcnt=0;
// uint8_t val_1,val_2;

/*********************************************************************************************************
* Function Name:        TIMER32_1_IRQHandler
* Description:          TIMER32_1 �жϴ���������������
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void TIMER32_1_IRQHandler (void)
{
//     static uint8_t i = 0;
//     if (i == 1) {
//         i = 0;
//     }
//     i++;
         LPC_TMR32B1->IR  = 1 << 4;                                      /* ���CAP0.0�жϱ�־           */
         GuiCapFlag = 1;                                                    //�жϱ�־
         LPC_TMR32B1->TCR     = 0x00;                                       //�رն�ʱ��
}

/*********************************************************************************************************
* Function Name:        TIMER16_0_IRQHandler
* Description:          TIMER16_0 �жϴ�������ƥ�����
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
    uint8_t valTemp[2],valHumi[2],i,checkUart;
    char error,checksum;
    
    SystemInit();                                                       /* ϵͳ��ʼ��������ɾ��         */
    GPIOInit();
    uartInit();
    timer0Init();
    timer1Init();
    myDelay(20);                                                        //�ϵ�֮����Ҫ�ȴ�11ms��Խ�������ߡ�״̬
    while (1) 
    {
//         //ds18b20����
//         DS18B20_Reset();
//         ds_write_byte(0xCC);
//         ds_write_byte(0x44);
//         myDelay(1);
//         DS18B20_Reset();
//         ds_write_byte(0xCC);
//         ds_write_byte(0xBE);
//         uartSendByte(0xAA);                                              //֡ͷ1
//         uartSendByte(0xBB);                                              //֡ͷ2
//         uartSendByte(0x03);                                              //�����򳤶ȣ�������ֽڣ�
//         uartSendByte(0x01);                                              //�����֣��ݶ�0x01Ϊ�¶ȣ� 
//         for(i=0;i<2;i++)
//         {
//             temperature[i] = ds_read_byte() ;
//             uartSendByte(temperature[i]);
//         }
//         DS18B20_Reset();    
//         myDelay(100);
//                 
        
        
        /** ��ʪ�ȴ��� **/
        temp_val.i=0;
        humi_val.i = 0;
        error=0;    
        error += s_measure((char*) &(temp_val.i),&checksum,TEMP);
        error += s_measure((char*)&(humi_val.i),&checksum,HUMI);
//         temp_val.f = (float)(temp_val.i);
//         humi_val.f = (float)(humi_val.i);
//         temp = -40+0.01*temp_val.f;
//         humi = -4+0.0405*humi_val.f-2.8/1000000*humi_val.f*humi_val.f;
        
        /** ��������ʱ���� **/
        LPC_TMR16B0->TCR = 0x01;                                            //�򿪶�ʱ��16_0������ʱ��ʼƥ�����180kHz����        
        LPC_TMR32B1->TCR = 0x01;                                            //�򿪶�ʱ��32_1����ʱ��ʼ�������룬�����ز����ж�(��init.c)
//         while(!GuiCapFlag);                                                 //�ȴ�ֱ�����������־λ��1
myDelay(1);
        timerCount = LPC_TMR32B1->CR0;                                      //������Ĵ�����ֵȡ��       
        GuiCapFlag  = 0;
        LPC_TMR32B1->TC      = 0;                                           //���������־λ��0
        
        /** UART������֡���� **/
        uartSendByte(0xAA);                                                  //֡ͷ1       
        uartSendByte(0xBB);                                                  //֡ͷ2
        checkUart = 0x00;                                                    //���У��λ       
        uartSendByte(lengthUart);                                            //����֡����
        checkUart ^= lengthUart;                                             //������֡����Ϊ���
        uartSendByte(cmdUartSend);                                           //������(1Byte)
        checkUart ^= cmdUartSend;                                            //�����������
        for(i=0;i<2;i++)
        {
            valTemp[i] = *((char*)(&temp_val.i)+i);                          //�¶�����(2Byte)
            checkUart ^= valTemp[i];                                         //���¶��������
            uartSendByte(valTemp[i]);
        }
        for(i=0;i<2;i++)
        {
            valHumi[i] = *((char*)(&humi_val.i)+i);                           //ʪ������(2Byte)
            checkUart ^= valHumi[i];                                          //��ʪ���������
            uartSendByte(valHumi[i]);
        }
//         timerCount = 0xEE;
        uartSendByte(timerCount);                                             //��ʱ����(2Byte)
        checkUart ^= timerCount;                                              //����ʱ�������
        uartSendByte(timerCount >> 8); 
        checkUart ^= timerCount >> 8;
        uartSendByte(checkUart);
        /** �ӳ�1s **/
        myDelay(60000);
    }
}

/*********************************************************************************************************
End Of File
*********************************************************************************************************/
