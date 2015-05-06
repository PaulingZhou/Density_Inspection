#include "LPC11xx.h"                                                    /* LPC11xx����Ĵ���            */
#include "init.h"                                                       /* ��Ҫ����myDelay()���� */
#include "ds18b20.h"
#include "uart.h"

/*********************************************************************************************************
�궨��
*********************************************************************************************************/
// #define DATAGIGH            (1ul << 11)         //DATA����ΪPIO2_11,DATAHIGH��ʾ�������߸ߵ�ƽ
// #define PIO2_11_FUNC        0x00                //��PIO2_11����ΪGPIOģʽ
// #define PIO2_11_MODE        0x02                //��PIO2_11����Ϊ�ڲ�����ʹ��
uint8_t state = 1;
uint8_t Temp=0;

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
    LPC_GPIO2->DIR  |= ds_DATAHIGH;                              /* ��p2.7,p2.8����Ĵ�����1(����Ϊ���) */
    LPC_GPIO2->DATA |= ds_DATAHIGH;                              /* ��p2.7,p2.8����Ϊ��ʼ������ߵ�ƽ */ 
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
    state = 1;
    LPC_GPIO2->DIR  |= ds_DATAHIGH;                     //�л�Ϊ���ģʽ 
    LPC_GPIO2->DATA &= ~ds_DATAHIGH;                    //�͵�ƽ
    myDelay_nop(1000);                                   //540us
    LPC_GPIO2->DATA |= ds_DATAHIGH;                     //�ߵ�ƽ
    LPC_GPIO2->DIR  &= ~ds_DATAHIGH;                    //�л�Ϊ����ģʽ
    myDelay_nop(150);                                   //84us
    state = (LPC_GPIO2->DATA >> 11)&0x01;
    while(!((LPC_GPIO2->DATA >> 11)&0x01));
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
    LPC_GPIO2->DIR  |= ds_DATAHIGH;                 //�������л�Ϊ���
    for(i=8;i>0;i--)
    {
        if(value&0x01)                                             //����λΪ1
        {
            LPC_GPIO2->DATA &= ~ds_DATAHIGH;                    //�͵�ƽ
            myDelay_nop(20);                                    //�ӳ�11.6us������15us֮���������ߵ�ƽ
            LPC_GPIO2->DATA |= ds_DATAHIGH;                     //�ߵ�ƽ
            myDelay_nop(100);                                   //�ӳ�55.6us����֮ǰ�͵�ƽʱ����������60us
        }
        else                                        //����λΪ0
        {
            LPC_GPIO2->DATA &= ~ds_DATAHIGH;                    //�͵�ƽ
            myDelay_nop(120);                                   //�ӳ�66us�������60us
        }
        value >>= 1;
        LPC_GPIO2->DATA |= ds_DATAHIGH;                     //�ߵ�ƽ
        myDelay_nop(5);                                     //�ӳ�4us�������4us
    }
}

/*********************************************************************************************************
** Function name:       ds_read_byte
** Descriptions:        ��ds18b20��һ���ֽڲ�����Ƿ��л�Ӧ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8_t ds_read_byte(void)
{
    uint8_t i,count;
    for(i=0;i<8;i++)
    {
        Temp >>= 1;
        LPC_GPIO2->DIR  |= ds_DATAHIGH;                 //�������л�Ϊ���
        LPC_GPIO2->DATA &= ~ds_DATAHIGH;                //�͵�ƽ
        for(count = 0;count<10;count++);
        LPC_GPIO2->DIR  &= ~ds_DATAHIGH;                //�������л�Ϊ����
        myDelay_nop(10);                                //�ӳ�8us��׼��������
        if((LPC_GPIO2->DATA >> 11)&0x01)                //LSB
            Temp |= 0x80;
        myDelay_nop(100);                                //�ӳ�60us
    }
//     uartSendByte(Temp);
    return Temp;
}
