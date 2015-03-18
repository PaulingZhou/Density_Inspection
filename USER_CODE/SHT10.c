#include "LPC11xx.h"                                                    /* LPC11xx外设寄存器            */
#include "main.h"

#define noACK               0
#define ACK                 1
                                                //adr command r/w
#define STATUS_REG_W 0x06                       //000  0011    0
#define STATUS_REG_R 0x07                       //000  0011    1
#define MEASURE_TEMP 0x03                       //000  0001    1
#define MEASURE_HUMI 0x05                       //000  0010    1
#define RESET 0x1e                              //000  1111    0

/*********************************************************************************************************
** Function name:       s_write_byte
** Descriptions:        从Sensibus上写一个字节并检查是否有回应
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
char s_write_byte(uint8_t value)
{
    uint8_t i,error = 0;
    LPC_GPIO2->DIR  |= DATAHIGH;                              /* 将模拟数据总线p2.7方向寄存器置1(配置为输出) */
    for(i=0x80;i>0;i/=2)                                       //shift bit for masking,0x80=1000 0000
    {
        myDelay(1);                                                //保持1ms(可省略，为总线上像一个时钟信号添加，后续可删除)            
        if (i & value) LPC_GPIO2->DATA |= DATAHIGH;              //masking value with i,write to SENSI-BUS
        else LPC_GPIO2->DATA &= ~DATAHIGH;                         //时钟低电平时改变数据总线数据(开始传输命令后SCK为低电平，故不必翻转)
        LPC_GPIO2->DATA |= SCKHIGH;                                //时钟高电平
        myDelay(1);                                                 //保持1ms
        LPC_GPIO2->DATA &= ~SCKHIGH;                               //时钟低电平
     }
     LPC_GPIO2->DATA |= DATAHIGH;                               //此时为第八个时钟低电平，释放数据总线(高电平)
     LPC_GPIO2->DIR  &= ~DATAHIGH;                              /* 将模拟数据总线p2.7方向寄存器置0(配置为输入) */
     myDelay(1);                                                //保持1ms
     LPC_GPIO2->DATA |= SCKHIGH;                                //时钟高电平(第8个下降沿，若传输成功SHT10将把DATA总线拉低(ACK)) 
     myDelay(1);                                                //保持1ms
     error= (LPC_GPIO2->DATA>>7);                               //校验，若error=1说明传输错误
     LPC_GPIO2->DATA &= ~SCKHIGH;                               //SCK低电平
     return error;
}

    
/*********************************************************************************************************
** Function name:       s_transstart
** Descriptions:        产生传输开始指令
** input parameters:    无
** output parameters:       __      _____
                      DATA:   |____|
                             __    __
                      SCK: _|  |__|  |___
** Returned value:      无
*********************************************************************************************************/
void s_transstart(void)
{
    LPC_GPIO2->DIR  |= DATAHIGH;                              /* 将模拟数据总线p2.7方向寄存器置1(配置为输出) */
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
    LPC_GPIO2->DIR  &= ~DATAHIGH;                              /* 将模拟数据总线p2.7方向寄存器置0(配置为输入) */
}
