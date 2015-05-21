#include "LPC11xx.h"                                                    /* LPC11xx����Ĵ���            */
#include "sht10.h"
#include "ds18b20.h"

/*********************************************************************************************************
  �궨��
*********************************************************************************************************/
#define UART_BPS    9600                                                /*  ����ͨ�Ų�����              */
// #define OUTPUT  (1ul << 3)
// #define OUTPUT_HIGH()   LPC_GPIO0->DATA |= OUTPUT                       //PIO0_3����ߵ�ƽ
// #define OUTPUT_LOW()    LPC_GPIO0->DATA &= ~OUTPUT                       //PIO0_3����͵�ƽ

/*********************************************************************************************************
** Function name:       myDelay
** Descriptions:        �����ʱ��ÿ��λ��ʱ1ms(�Դ�)
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:        �����ʱ��ÿ��λ��ʱ1us(�Դ�)
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:        GPIO��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void GPIOInit( void )
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* ��ʼ��GPIO AHBʱ��       */
    LPC_IOCON->PIO2_7 = PIO2_7_FUNC|(PIO2_MODE<<3);                     /* ��P2.7����ΪGPIO���ܣ���������ʹ��      */
    LPC_IOCON->PIO2_8 &= PIO2_8_FUNC|(PIO2_MODE<<3);                    /* ��p2.8����ΪGPIO���ܣ���������ʹ��       */
    LPC_GPIO2->DIR  |= (DATAHIGH|SCKHIGH);                              /* ��p2.7,p2.8����Ĵ�����1(����Ϊ���) */
    LPC_GPIO2->DATA |= (DATAHIGH|SCKHIGH);                              /* ��p2.7,p2.8����Ϊ��ʼ������ߵ�ƽ */  
    ds18b20Init();                                                      //DS18b20 IO ����(PIO2_11)
//     LPC_IOCON->PIO0_3 |= 0x00 | (0x02<<3);
//     LPC_GPIO0->DIR |= OUTPUT;   
}

/*********************************************************************************************************
** Function name:       uartInit
** Descriptions:        ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600��P1.6ΪRXD��P1.7ΪTXD
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartInit (void)
{
    uint16_t usFdiv;
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);
    LPC_IOCON->PIO1_5 |= 0x01;
    LPC_IOCON->PIO1_6 |= 0x01;
    LPC_IOCON->PIO1_7 |= 0x01;

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);                               /* ��UART���ܲ���ʱ��         */
    LPC_SYSCON->UARTCLKDIV       = 0x01;                                /* UARTʱ�ӷ�Ƶ                 */

    LPC_UART->LCR  = 0x83;                                              /* �������ò�����               */
    usFdiv = (SystemFrequency/LPC_SYSCON->UARTCLKDIV/16)/UART_BPS;      /* ���ò�����                   */
    LPC_UART->DLM  = usFdiv / 256;
    LPC_UART->DLL  = usFdiv % 256; 
    LPC_UART->LCR  = 0x03;                                              /* ����������                   */
    LPC_UART->FCR  = 0x07;
    LPC_UART->RS485CTRL = 0x30;                                         /* ����485                      */
}

/*********************************************************************************************************
** Function name:       timer0Init
** Descriptions:        16λ��ʱ��0��ʼ��������ʹ��ƥ���������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void timer0Init (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);                             /* ��IOCONģ��ʱ��            */
    LPC_IOCON->PIO0_8 |= 0x02;                                          /* ��P0.8����ΪMAT0�������     */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 7);                              /* ��16λ��ʱ��0ʱ��ģ��      */
    LPC_TMR16B0->PR      = 0;                                           /* ���÷�Ƶϵ��                 */
    LPC_TMR16B0->MCR     = (0x03);                                   /* ����MR0ƥ���λTC���������ж�          */
    LPC_TMR16B0->EMR     = (0x03 << 4) ;                                /* MR0ƥ���MAT0.0�����ת      */
    LPC_TMR16B0->MR0     = 132;                                         /* Ƶ�ʿ��ƣ�180kHz   */
    LPC_TMR16B0->TCR     = 0x01;                                        /* ������ʱ��                   */
    NVIC_EnableIRQ(TIMER_16_0_IRQn);                                    /* �����жϲ�ʹ��               */
    NVIC_SetPriority(TIMER_16_0_IRQn, 2);
}

/*********************************************************************************************************
** Function name:       timer1Init
** Descriptions:        32λ��ʱ��1��ʼ��������ʹ�ò��������ʱ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void timer1Init (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);                               /* ��IOCONģ��ʱ��         */
    LPC_IOCON->R_PIO1_0 |= 0x03;                                            /* ��P1.0����ΪCAP0��������  */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10);                                /* ��16λ��ʱ��1ʱ��ģ��   */
    LPC_TMR32B1->PR         =0;
    LPC_TMR32B1->CCR     = 0x06;                                      /* ����CAP0.0�½��ز���0110         */
    LPC_TMR32B1->TC      = 0;
    LPC_TMR32B1->TCR     = 0x01;                                        /* ������ʱ��                   */
    NVIC_EnableIRQ(TIMER_32_1_IRQn);
    NVIC_SetPriority(TIMER_32_1_IRQn, 2);
}
