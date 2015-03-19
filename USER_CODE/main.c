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

/*********************************************************************************************************
�궨��
*********************************************************************************************************/
#define DATAHIGH            (1ul << 7)           //DATA����ΪPIO2_7,DATABHIGH��ʾ�������߸ߵ�ƽ
#define SCKHIGH             (1ul << 8)           //SCKΪ����PIO2_8,SCKHIGH��ʾʱ���߸ߵ�ƽ
#define PIO2_7_FUNC         0x00                 //��PIO2_7����ΪGPIO����
#define PIO2_8_FUNC         0x00                 //��PIO2_8����ΪGPIO����
#define PIO2_MODE           0x02                 //��PIO2_7,PIO2_8����Ϊ�ڲ�����ʹ�ܣ���ʵ���ⲿ�����5V��10k����������ܱ�֤���ת����ʱ����������Եĵ�ƽ�½�

#define noACK               0
#define ACK                 1
                                                //adr command r/w
#define STATUS_REG_W 0x06                       //000  0011    0
#define STATUS_REG_R 0x07                       //000  0011    1
#define MEASURE_TEMP 0x03                       //000  0001    1
#define MEASURE_HUMI 0x05                       //000  0010    1
#define RESET 0x1e                              //000  1111    0

uint8_t val=0;

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
** Function name:       main
** Descriptions:        ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int main (void)
{
    
    SystemInit();                                                       /* ϵͳ��ʼ��������ɾ��         */
    GPIOInit();
    myDelay(20);                    //�ϵ�֮����Ҫ�ȴ�11ms��Խ�������ߡ�״̬
//     s_transstart();
//     s_write_byte(6);
//     s_write_byte(2);
    while (1) 
    {
        s_connectionreset();
//         s_transstart();
//         s_write_byte(7);
//         val = s_read_byte(noACK);
//         for(i=0;i<10;i++)
//         {
//             myDelay(1);
//             LPC_GPIO2->DATA |= SCKHIGH;
//             myDelay(1);
//             LPC_GPIO2->DATA &= ~SCKHIGH;
//         }
    }
}

/*********************************************************************************************************
End Of File
*********************************************************************************************************/
