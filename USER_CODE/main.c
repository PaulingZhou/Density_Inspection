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
#include "ds18b20.h"

#define lengthUart 0x07                                //宏定义 数据帧长度 除了帧头外所有数据位的长度，现为长度(1byte)，命令字(1byte)，
                                                       //温度，湿度，声时(各2byte)，校验(1bit)
#define cmdUartSend 0x07                               //宏定义 命令字 第0位表示温度，第1位表示湿度，第2位标识声时，置1表示发送

typedef union
{
    uint32_t i;
    float f;
}value;
float temp,humi;
value humi_val,temp_val;
uint32_t timerCount = 0;
volatile uint8_t GuiCapFlag = 0;                                     /* 定时器捕获中断标志           */
uint32_t i,tmr0_opcnt=0;
// uint8_t val_1,val_2;

/*********************************************************************************************************
* Function Name:        TIMER32_1_IRQHandler
* Description:          TIMER32_1 中断处理函数，捕获输入
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void TIMER32_1_IRQHandler (void)
{
//     static uint8_t i = 0;
//     if (i == 1) {
//         i = 0;
//     }
//     i++;
         LPC_TMR32B1->IR  = 1 << 4;                                      /* 清除CAP0.0中断标志           */
         GuiCapFlag = 1;                                                    //中断标志
         LPC_TMR32B1->TCR     = 0x00;                                       //关闭定时器
}

/*********************************************************************************************************
* Function Name:        TIMER16_0_IRQHandler
* Description:          TIMER16_0 中断处理函数，匹配输出
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void TIMER16_0_IRQHandler (void)
{
    LPC_TMR16B0->IR = 0x01;                                             /* 清除中断标志                 */
    tmr0_opcnt++;
    if(tmr0_opcnt == 10){
        LPC_TMR16B0->TCR = 0x00;
        tmr0_opcnt = 0;        
    }
}

/*********************************************************************************************************
** Function name:       main
** Descriptions:        无
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main (void)
{
    uint8_t valTemp[2],valHumi[2],i,checkUart;
    char error,checksum;
    
    SystemInit();                                                       /* 系统初始化，切勿删除         */
    GPIOInit();
    uartInit();
    timer0Init();
    timer1Init();
    myDelay(20);                                                        //上电之后需要等待11ms以越过“休眠”状态
    while (1) 
    {
//         //ds18b20代码
//         DS18B20_Reset();
//         ds_write_byte(0xCC);
//         ds_write_byte(0x44);
//         myDelay(1);
//         DS18B20_Reset();
//         ds_write_byte(0xCC);
//         ds_write_byte(0xBE);
//         uartSendByte(0xAA);                                              //帧头1
//         uartSendByte(0xBB);                                              //帧头2
//         uartSendByte(0x03);                                              //数据域长度（不算该字节）
//         uartSendByte(0x01);                                              //命令字（暂定0x01为温度） 
//         for(i=0;i<2;i++)
//         {
//             temperature[i] = ds_read_byte() ;
//             uartSendByte(temperature[i]);
//         }
//         DS18B20_Reset();    
//         myDelay(100);
//                 
        
        
        /** 温湿度代码 **/
        temp_val.i=0;
        humi_val.i = 0;
        error=0;    
        error += s_measure((char*) &(temp_val.i),&checksum,TEMP);
        error += s_measure((char*)&(humi_val.i),&checksum,HUMI);
//         temp_val.f = (float)(temp_val.i);
//         humi_val.f = (float)(humi_val.i);
//         temp = -40+0.01*temp_val.f;
//         humi = -4+0.0405*humi_val.f-2.8/1000000*humi_val.f*humi_val.f;
        
        /** 超声波计时代码 **/
        LPC_TMR16B0->TCR = 0x01;                                            //打开定时器16_0，即此时开始匹配输出180kHz脉冲        
        LPC_TMR32B1->TCR = 0x01;                                            //打开定时器32_1，此时开始捕获输入，上升沿产生中断(见init.c)
//         while(!GuiCapFlag);                                                 //等待直到捕获输入标志位置1
myDelay(1);
        timerCount = LPC_TMR32B1->CR0;                                      //将捕获寄存器的值取出       
        GuiCapFlag  = 0;
        LPC_TMR32B1->TC      = 0;                                           //捕获输入标志位置0
        
        /** UART将数据帧发出 **/
        uartSendByte(0xAA);                                                  //帧头1       
        uartSendByte(0xBB);                                                  //帧头2
        checkUart = 0x00;                                                    //清空校验位       
        uartSendByte(lengthUart);                                            //数据帧长度
        checkUart ^= lengthUart;                                             //与数据帧长度为异或
        uartSendByte(cmdUartSend);                                           //命令字(1Byte)
        checkUart ^= cmdUartSend;                                            //与命令字异或
        for(i=0;i<2;i++)
        {
            valTemp[i] = *((char*)(&temp_val.i)+i);                          //温度数据(2Byte)
            checkUart ^= valTemp[i];                                         //与温度数据异或
            uartSendByte(valTemp[i]);
        }
        for(i=0;i<2;i++)
        {
            valHumi[i] = *((char*)(&humi_val.i)+i);                           //湿度数据(2Byte)
            checkUart ^= valHumi[i];                                          //与湿度数据异或
            uartSendByte(valHumi[i]);
        }
//         timerCount = 0xEE;
        uartSendByte(timerCount);                                             //声时数据(2Byte)
        checkUart ^= timerCount;                                              //与声时数据异或
        uartSendByte(timerCount >> 8); 
        checkUart ^= timerCount >> 8;
        uartSendByte(checkUart);
        /** 延迟1s **/
        myDelay(60000);
    }
}

/*********************************************************************************************************
End Of File
*********************************************************************************************************/
