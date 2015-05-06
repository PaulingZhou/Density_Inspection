#include "LPC11xx.h"                                                    /* LPC11xx外设寄存器            */
#include "init.h"                                                       /* 主要调用myDelay()函数 */
#include "ds18b20.h"
#include "uart.h"

/*********************************************************************************************************
宏定义
*********************************************************************************************************/
// #define DATAGIGH            (1ul << 11)         //DATA总线为PIO2_11,DATAHIGH表示数据总线高电平
// #define PIO2_11_FUNC        0x00                //将PIO2_11设置为GPIO模式
// #define PIO2_11_MODE        0x02                //将PIO2_11设置为内部上拉使能
uint8_t state = 1;
uint8_t Temp=0;

/*********************************************************************************************************
** Function name:       ds18b20Init
** Descriptions:        温度传感器ds18b20初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ds18b20Init(void)
{
    LPC_IOCON->PIO2_11 = PIO2_11_FUNC|(PIO2_11_MODE<<3);        /* 将P2.11定义为GPIO功能，上拉电阻使能      */
    LPC_GPIO2->DIR  |= ds_DATAHIGH;                              /* 将p2.7,p2.8方向寄存器置1(配置为输出) */
    LPC_GPIO2->DATA |= ds_DATAHIGH;                              /* 将p2.7,p2.8配置为初始化输出高电平 */ 
}

/*********************************************************************************************************
** Function name:       DS18B20_Reset()
** Descriptions:        DS18B20复位及存在检测
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void DS18B20_Reset(void)
{
    state = 1;
    LPC_GPIO2->DIR  |= ds_DATAHIGH;                     //切换为输出模式 
    LPC_GPIO2->DATA &= ~ds_DATAHIGH;                    //低电平
    myDelay_nop(1000);                                   //540us
    LPC_GPIO2->DATA |= ds_DATAHIGH;                     //高电平
    LPC_GPIO2->DIR  &= ~ds_DATAHIGH;                    //切换为输入模式
    myDelay_nop(150);                                   //84us
    state = (LPC_GPIO2->DATA >> 11)&0x01;
    while(!((LPC_GPIO2->DATA >> 11)&0x01));
}

/*********************************************************************************************************
** Function name:       ds_write_byte
** Descriptions:        往ds18b20写一个字节并检查是否有回应
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ds_write_byte(uint8_t value)
{
    uint8_t i=0;
    LPC_GPIO2->DIR  |= ds_DATAHIGH;                 //将方向切换为输出
    for(i=8;i>0;i--)
    {
        if(value&0x01)                                             //若该位为1
        {
            LPC_GPIO2->DATA &= ~ds_DATAHIGH;                    //低电平
            myDelay_nop(20);                                    //延迟11.6us，需在15us之内上升到高电平
            LPC_GPIO2->DATA |= ds_DATAHIGH;                     //高电平
            myDelay_nop(100);                                   //延迟55.6us，与之前低电平时间相加需大于60us
        }
        else                                        //若该位为0
        {
            LPC_GPIO2->DATA &= ~ds_DATAHIGH;                    //低电平
            myDelay_nop(120);                                   //延迟66us，需大于60us
        }
        value >>= 1;
        LPC_GPIO2->DATA |= ds_DATAHIGH;                     //高电平
        myDelay_nop(5);                                     //延迟4us，需大于4us
    }
}

/*********************************************************************************************************
** Function name:       ds_read_byte
** Descriptions:        往ds18b20读一个字节并检查是否有回应
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint8_t ds_read_byte(void)
{
    uint8_t i,count;
    for(i=0;i<8;i++)
    {
        Temp >>= 1;
        LPC_GPIO2->DIR  |= ds_DATAHIGH;                 //将方向切换为输出
        LPC_GPIO2->DATA &= ~ds_DATAHIGH;                //低电平
        for(count = 0;count<10;count++);
        LPC_GPIO2->DIR  &= ~ds_DATAHIGH;                //将方向切换为输入
        myDelay_nop(10);                                //延迟8us，准备读数据
        if((LPC_GPIO2->DATA >> 11)&0x01)                //LSB
            Temp |= 0x80;
        myDelay_nop(100);                                //延迟60us
    }
//     uartSendByte(Temp);
    return Temp;
}
