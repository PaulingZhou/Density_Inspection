#include "LPC11xx.h"                                                    /* LPC11xx外设寄存器            */
#include "sht10.h"

/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define UART_BPS    9600                                                /*  串口通信波特率              */

/*********************************************************************************************************
** Function name:       myDelay
** Descriptions:        软件延时，每单位延时1ms(略大)
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void myDelay (uint32_t ulTime)
{
    uint32_t i;
    i = 0;
    while (ulTime--)
    {
        for (i = 0; i < 3300; i++)
        __nop();
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
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* 初始化GPIO AHB时钟       */
    LPC_IOCON->PIO2_7 = PIO2_7_FUNC|(PIO2_MODE<<3);                     /* 将P2.7定义为GPIO功能，上拉电阻使能      */
    LPC_IOCON->PIO2_8 &= PIO2_8_FUNC|(PIO2_MODE<<3);                    /* 将p2.8定义为GPIO功能，上拉电阻使能       */
    LPC_GPIO2->DIR  |= (DATAHIGH|SCKHIGH);                              /* 将p2.7,p2.8方向寄存器置1(配置为输出) */
    LPC_GPIO2->DATA |= (DATAHIGH|SCKHIGH);                              /* 将p2.7,p2.8配置为初始化输出高电平 */          
}

/*********************************************************************************************************
** Function name:       uartInit
** Descriptions:        串口初始化，设置为8位数据位，1位停止位，无奇偶校验，波特率为9600
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartInit (void)
{
    uint16_t usFdiv;
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);
    LPC_IOCON->PIO1_5 |= 0x01;
    LPC_IOCON->PIO1_6 |= 0x01;
    LPC_IOCON->PIO1_7 |= 0x01;

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);                               /* 打开UART功能部件时钟         */
    LPC_SYSCON->UARTCLKDIV       = 0x01;                                /* UART时钟分频                 */

    LPC_UART->LCR  = 0x83;                                              /* 允许设置波特率               */
    usFdiv = (SystemFrequency/LPC_SYSCON->UARTCLKDIV/16)/UART_BPS;      /* 设置波特率                   */
    LPC_UART->DLM  = usFdiv / 256;
    LPC_UART->DLL  = usFdiv % 256; 
    LPC_UART->LCR  = 0x03;                                              /* 锁定波特率                   */
    LPC_UART->FCR  = 0x07;
    LPC_UART->RS485CTRL = 0x30;                                         /* 配置485                      */
}
