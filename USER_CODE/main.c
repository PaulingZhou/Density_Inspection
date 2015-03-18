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
** Function name:       s_write_byte
** Descriptions:        ��Sensibus��дһ���ֽڲ�����Ƿ��л�Ӧ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
char s_write_byte(uint8_t value)
    {
     uint8_t i,error = 0;
     LPC_GPIO2->DIR  |= DATAHIGH;                              /* ��ģ����������p2.7����Ĵ�����1(����Ϊ���) */
     for(i=0x80;i>0;i/=2)                                       //shift bit for masking,0x80=1000 0000
        {
         myDelay(1);                                                //����1ms(��ʡ�ԣ�Ϊ��������һ��ʱ���ź���ӣ�������ɾ��)            
         if (i & value) LPC_GPIO2->DATA |= DATAHIGH;              //masking value with i,write to SENSI-BUS
         else LPC_GPIO2->DATA &= ~DATAHIGH;                         //ʱ�ӵ͵�ƽʱ�ı�������������(��ʼ���������SCKΪ�͵�ƽ���ʲ��ط�ת)
         LPC_GPIO2->DATA |= SCKHIGH;                                //ʱ�Ӹߵ�ƽ
         myDelay(1);                                                 //����1ms
         LPC_GPIO2->DATA &= ~SCKHIGH;                               //ʱ�ӵ͵�ƽ
        }
     LPC_GPIO2->DATA |= DATAHIGH;                               //��ʱΪ�ڰ˸�ʱ�ӵ͵�ƽ���ͷ���������(�ߵ�ƽ)
     LPC_GPIO2->DIR  &= ~DATAHIGH;                              /* ��ģ����������p2.7����Ĵ�����0(����Ϊ����) */
     myDelay(1);                                                //����1ms
     LPC_GPIO2->DATA |= SCKHIGH;                                //ʱ�Ӹߵ�ƽ(��8���½��أ�������ɹ�SHT10����DATA��������(ACK)) 
     myDelay(1);                                                //����1ms
     error= (LPC_GPIO2->DATA>>7);                               //У�飬��error=1˵���������
     LPC_GPIO0->DATA &= ~SCKHIGH;                               //�ߵ�ƽ
     return error;
    }



/*********************************************************************************************************
** Function name:       s_transstart
** Descriptions:        �������俪ʼָ��
** input parameters:    ��
** output parameters:     __      _____
                    DATA:   |____|
                           __    __
                    SCK: _|  |__|  |___
** Returned value:      ��
*********************************************************************************************************/
void s_transstart(void){
    LPC_GPIO2->DIR  |= DATAHIGH;                              /* ��ģ����������p2.7����Ĵ�����1(����Ϊ���) */
    LPC_GPIO2->DATA |= DATAHIGH;        //DATA=1
    LPC_GPIO2->DATA &= ~SCKHIGH;        //SCK=0
    myDelay(1);
    LPC_GPIO2->DATA |= SCKHIGH;         //SCK=1
    myDelay(1);
    LPC_GPIO2->DATA &= ~DATAHIGH;       //DATA=0
    myDelay(1);
    LPC_GPIO2->DATA &= ~SCKHIGH;        //SCK=0
    myDelay(3);
    LPC_GPIO2->DATA |= SCKHIGH;         //SCK=1
    myDelay(1);
    LPC_GPIO2->DATA |= DATAHIGH;        //DATA=1
    myDelay(1);
    LPC_GPIO2->DATA &= ~SCKHIGH;        //SCK=0
    myDelay(1);
    LPC_GPIO2->DIR  &= ~DATAHIGH;                              /* ��ģ����������p2.7����Ĵ�����0(����Ϊ����) */
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
while (1) 
    {
     s_transstart();
     s_write_byte(160);
//         myDelay(10);
//             myDelay(1);
//             LPC_GPIO2->DATA |= DATAHIGH;
//             myDelay(1);
//             LPC_GPIO2->DATA &= ~DATAHIGH;
//             __nop();__nop();__nop();__nop;__nop();__nop();
}
}

/*********************************************************************************************************
End Of File
*********************************************************************************************************/
