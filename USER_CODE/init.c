#include "LPC11xx.h"                                                    /* LPC11xx外设寄存器            */
#include "sht10.h"
#include "ds18b20.h"

/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define UART_BPS    9600                                                /*  串口通信波特率              */
// #define OUTPUT  (1ul << 3)
// #define OUTPUT_HIGH()   LPC_GPIO0->DATA |= OUTPUT                       //PIO0_3输出高电平
// #define OUTPUT_LOW()    LPC_GPIO0->DATA &= ~OUTPUT                       //PIO0_3输出低电平

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
** Function name:       myDelay
** Descriptions:        软件延时，每单位延时1us(略大)
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
 void myDelay_nop (uint32_t ulTime)
{
    uint32_t i;
    i = 0;
    while (ulTime--)
    {
        for (i = 0; i < 1; i++)
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
    ds18b20Init();                                                      //DS18b20 IO 配置(PIO2_11)
//     LPC_IOCON->PIO0_3 |= 0x00 | (0x02<<3);
//     LPC_GPIO0->DIR |= OUTPUT;   
}

/*********************************************************************************************************
** Function name:       uartInit
** Descriptions:        串口初始化，设置为8位数据位，1位停止位，无奇偶校验，波特率为9600，P1.6为RXD，P1.7为TXD
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

/*********************************************************************************************************
** Function name:       timer0Init
** Descriptions:        16位定时器0初始化函数，使用匹配输出脉冲
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void timer0Init (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);                             /* 打开IOCON模块时钟            */
    LPC_IOCON->PIO0_8 |= 0x02;                                          /* 将P0.8配置为MAT0输出引脚     */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 7);                              /* 打开16位定时器0时钟模块      */
    LPC_TMR16B0->PR      = 0;                                           /* 设置分频系数                 */
    LPC_TMR16B0->MCR     = (0x03);                                   /* 设置MR0匹配后复位TC，并产生中断          */
    LPC_TMR16B0->EMR     = (0x03 << 4) ;                                /* MR0匹配后MAT0.0输出翻转      */
    LPC_TMR16B0->MR0     = 132;                                         /* 频率控制，180kHz   */
    LPC_TMR16B0->TCR     = 0x01;                                        /* 启动定时器                   */
    NVIC_EnableIRQ(TIMER_16_0_IRQn);                                    /* 设置中断并使能               */
    NVIC_SetPriority(TIMER_16_0_IRQn, 2);
}

/*********************************************************************************************************
** Function name:       timer1Init
** Descriptions:        32位定时器1初始化函数，使用捕获输入计时
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void timer1Init (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);                               /* 打开IOCON模块时钟         */
    LPC_IOCON->R_PIO1_0 |= 0x03;                                            /* 将P1.0配置为CAP0输入引脚  */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10);                                /* 打开16位定时器1时钟模块   */
    LPC_TMR32B1->PR         =0;
    LPC_TMR32B1->CCR     = 0x06;                                      /* 设置CAP0.0下降沿捕获，0110         */
    LPC_TMR32B1->TC      = 0;
    LPC_TMR32B1->TCR     = 0x01;                                        /* 启动定时器                   */
    NVIC_EnableIRQ(TIMER_32_1_IRQn);
    NVIC_SetPriority(TIMER_32_1_IRQn, 2);
}
