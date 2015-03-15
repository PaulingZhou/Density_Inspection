/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Development Co., LTD
**
**                                 http://www.zlgmcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2010-02-04
** Last Version:        V1.0
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Li Baihua
** Created date:        2010-09-10
** Version:             V1.00
** Descriptions:        整理模板，添加用户应用程序
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

/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define BEEP            (1ul << 7)

/*********************************************************************************************************
** Function name:       myDelay
** Descriptions:        软件延时
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void myDelay (uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}

/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void GPIOInit( void )
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* 初始化GPIO AHB时钟           */
    LPC_IOCON->PIO2_7 &= ~0x07;                                         /* 将P2.7初始化为GPIO功能       */
	LPC_GPIO2->DIR    |= BEEP;                                          /* 将P2.7方向设置为输出         */
	LPC_GPIO2->DATA   |= BEEP;                                          /* 将P2.7初始化输出高电平       */
}

/*********************************************************************************************************
** Function name:       main
** Descriptions:        BEEP测试程序
**                      跳线连接：短接P2.7与蜂鸣器BEEP控制引脚；
**                      现    象：下载代码，运行程序，则BEEP交替鸣叫
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main (void)
{
    SystemInit();                                                       /* 系统初始化，切勿删除         */

    GPIOInit();
    while (1) {
        LPC_GPIO2->DATA |= BEEP;
        myDelay(400); 
        LPC_GPIO2->DATA &= (~BEEP);
        myDelay(400);
    }
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
