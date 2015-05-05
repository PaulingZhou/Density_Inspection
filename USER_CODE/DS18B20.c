#include "LPC11xx.h"                                                    /* LPC11xx����Ĵ���            */
#include "init.h"                                                       /* ��Ҫ����myDelay()���� */
#include "ds18b20.h"

/*********************************************************************************************************
�궨��
*********************************************************************************************************/
// #define DATAGIGH            (1ul << 11)         //DATA����ΪPIO2_11,DATAHIGH��ʾ�������߸ߵ�ƽ
// #define PIO2_11_FUNC        0x00                //��PIO2_11����ΪGPIOģʽ
// #define PIO2_11_MODE        0x02                //��PIO2_11����Ϊ�ڲ�����ʹ��

/*********************************************************************************************************
** Function name:       ds18b20Init
** Descriptions:        �¶ȴ�����ds18b20��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ds18b20Init(void)
{
    LPC_IOCON->PIO2_11 = PIO2_11_FUNC|(PIO2_11_MODE<<3);        /* ��P2.11����ΪGPIO���ܣ���������ʹ��      */
    LPC_GPIO2->DIR  |= ds_DATAGIGH;                              /* ��p2.7,p2.8����Ĵ�����1(����Ϊ���) */
    LPC_GPIO2->DATA |= ds_DATAGIGH;                              /* ��p2.7,p2.8����Ϊ��ʼ������ߵ�ƽ */ 
}

/*********************************************************************************************************
** Function name:       DS18B20_Reset()
** Descriptions:        DS18B20��λ�����ڼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DS18B20_Reset(void)
{
//     char state = 1;
    LPC_GPIO2->DIR  |= ds_DATAGIGH;
    LPC_GPIO2->DATA |= ds_DATAGIGH; 
    LPC_GPIO2->DATA &= ~ds_DATAGIGH;                    //�͵�ƽ
    myDelay_nop(500);                                   //500us
//     LPC_GPIO2->DATA |= ds_DATAGIGH;                     //�ߵ�ƽ
    LPC_GPIO2->DIR  &= ~ds_DATAGIGH;
}

/*********************************************************************************************************
** Function name:       ds_write_byte
** Descriptions:        ��ds18b20дһ���ֽڲ�����Ƿ��л�Ӧ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ds_write_byte(uint8_t value)
{
    uint8_t i=0;
    LPC_GPIO2->DIR  |= ds_DATAGIGH;                 //�������л�Ϊ���
    for(i=0x80;i>0;i/=2)
    {
        if(i&value)                                             //����λΪ1
        {
            LPC_GPIO2->DATA &= ~ds_DATAGIGH;                    //�͵�ƽ
            myDelay_nop(20);                                    //�ӳ�11.6us������15us֮���������ߵ�ƽ
            LPC_GPIO2->DATA |= ds_DATAGIGH;                     //�ߵ�ƽ
            myDelay_nop(100);                                   //�ӳ�55.6us����֮ǰ�͵�ƽʱ����������60us
        }
        if(!(i&value))                                          //����λΪ0
        {
            LPC_GPIO2->DATA &= ~ds_DATAGIGH;                    //�͵�ƽ
            myDelay_nop(120);                                   //�ӳ�66us�������60us
        }
        LPC_GPIO2->DATA |= ds_DATAGIGH;                     //�ߵ�ƽ
        myDelay_nop(5);                                     //�ӳ�4us�������4us
    }
}
