#include "LPC11xx.h"                                                    /* LPC11xx外设寄存器            */
#include "init.h"                                                       /* 主要调用myDelay()函数 */
#include "sht10.h"

/*********************************************************************************************************
宏定义
*********************************************************************************************************/
// #define DATAHIGH            (1ul << 7)           //DATA总线为PIO2_7,DATABHIGH表示数据总线高电平
// #define SCKHIGH             (1ul << 8)           //SCK为总线PIO2_8,SCKHIGH表示时钟线高电平
// #define PIO2_7_FUNC         0x00                 //把PIO2_7设置为GPIO功能
// #define PIO2_8_FUNC         0x00                 //把PIO2_8设置为GPIO功能
// #define PIO2_MODE           0x02                 //把PIO2_7,PIO2_8设置为内部上拉使能，但实测外部仍需接5V与10k上拉电阻才能保证输出转输入时不会产生明显的电平下降

// #define noACK               0
// #define ACK                 1
//                                                 //adr command r/w
// #define STATUS_REG_W 0x06                       //000  0011    0
// #define STATUS_REG_R 0x07                       //000  0011    1
// #define MEASURE_TEMP 0x03                       //000  0001    1
// #define MEASURE_HUMI 0x05                       //000  0010    1
// #define RESET 0x1e                              //000  1111    0

uint8_t val_1,val_2,val_3,val_4;

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

/*********************************************************************************************************
** Function name:       s_write_byte
** Descriptions:        往Sensibus上写一个字节并检查是否有回应
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
     myDelay(1);
     return error;
}

/*********************************************************************************************************
** Function name:       s_read_byte
** Descriptions:        从Sensibus上读一个字节并给出回应
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
char s_read_byte(uint8_t ack)
{
    uint8_t i,val=0;
    LPC_GPIO2->DATA |= DATAHIGH;            //释放DATA数据线(若P2_7处于输入模式且配置为高阻状态则可注释)
    LPC_GPIO2->DIR &= ~DATAHIGH;            //切换为输入模式
    for(i=0x80;i>0;i/=2)                    //总共8bit，先读高位
    {
        LPC_GPIO2->DATA |= SCKHIGH;        //SCK高电平，此时可以读取数据
        myDelay(1);                         //延时1ms
        if(LPC_GPIO2->DATA&DATAHIGH) val = (val|i);            //若DATA高电平则说明该位为1，大端模式，先读高位
        LPC_GPIO2->DATA &= ~SCKHIGH;        //SCK低电平，此时sht10改变DATA电平
        myDelay(1);                         // 延时1ms
    }
    LPC_GPIO2->DIR |= DATAHIGH;             //DATA切换为输出模式
    switch(ack)
    {
        case ACK:                           //若为ACK
            LPC_GPIO2->DATA &= ~DATAHIGH;   //DATA低电平，表明确认该字节
            break;
        case noACK:                         //若为noACK
            LPC_GPIO2->DATA |= DATAHIGH;    //ack位DATA高电平，不使用CRC-8校验，中止通讯
            break;  
    }
    LPC_GPIO2->DATA |= SCKHIGH;             //CLK高电平，sht10读取ack位
    myDelay(1);                             //延时1ms
    LPC_GPIO2->DATA &= ~SCKHIGH;            //CLK低电平，之后控制器释放DATA
    LPC_GPIO2->DATA |= DATAHIGH;            //DATA高电平，控制器释放DATA，若在高阻状态可注释
    LPC_GPIO2->DIR &= ~DATAHIGH;            //DATA切换为输入模式
    myDelay(1);
    return val;                             //返回测量值(uint8_t类型变量)
}

/*********************************************************************************************************
** Function name:       s_connectionreset(void)
** Descriptions:        通讯复位时序
** input parameters:    无
                           ________________________________________     _____
** output parameters: DATA:                                        |___|
                             _   _   _   _   _   _   _   _   _     _   _
                      SCK: _| |_| |_| |_| |_| |_| |_| |_| |_| |___| |_| |____
** Returned value:      无
*********************************************************************************************************/
void s_connectionreset(void)
{
    uint8_t i;
    LPC_GPIO2->DIR |= (DATAHIGH|SCKHIGH);
    LPC_GPIO2->DATA |= DATAHIGH;
    LPC_GPIO2->DATA &= ~SCKHIGH;
    myDelay(1);
    for(i=0;i<9;i++)
    {
        LPC_GPIO2->DATA |= SCKHIGH;
        myDelay(1);
        LPC_GPIO2->DATA &= ~SCKHIGH;
        myDelay(1);
    }
    myDelay(2);
    s_transstart();
}

/*********************************************************************************************************
** Function name:       s_softreset(void)
** Descriptions:        软件复位传感器
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
char s_softreset(void)
{
    uint8_t error=0;
    s_connectionreset();            //reset communication
    error+=s_write_byte(RESET);     //send RESET-command(0x1F) to sensor
    return error;                   //error=1 in case of no response from the sensor
}

/*********************************************************************************************************
** Function name:       s_measure(uint8_t *p_value,uint8_t *checksum,uint8_t mode)
** Descriptions:        测量温湿度
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
char s_measure(char *p_value,char *checksum,uint8_t mode)
{
    char error = 0;
    uint32_t i;
    s_transstart();
    switch(mode)
    {
        case TEMP : error += s_write_byte(MEASURE_TEMP);break;
        case HUMI : error += s_write_byte(MEASURE_HUMI);break;
        default     : break;
    }
    for(i=0;i<2000;i++)
    {
        myDelay(1);
        if(!(LPC_GPIO2->DATA & DATAHIGH))break;
    }
    if(LPC_GPIO2->DATA & DATAHIGH) error+=1;
    *(p_value+1) = s_read_byte(ACK);
    *(p_value) = s_read_byte(noACK);
    val_1 = *(p_value);
    val_2 = *(p_value+1);
    val_3 = *(p_value+2);
    val_4 = *(p_value+3);
    return error;
}
