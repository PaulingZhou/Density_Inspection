#include "LPC11xx.h"                                                    /* LPC11xx����Ĵ���            */
#include "sht10.h"

/*********************************************************************************************************
  �궨��
*********************************************************************************************************/
#define UART_BPS    9600                                                /*  ����ͨ�Ų�����              */

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
}

/*********************************************************************************************************
** Function name:       uartInit
** Descriptions:        ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600
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
