#include "LPC11xx.h"                                                    /* LPC11xx外设寄存器            */
#include "sht10.h"

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
