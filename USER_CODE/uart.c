#include "LPC11xx.h" 

/*********************************************************************************************************
** Function name:       uartGetByte
** Descriptions:        从串口接收1字节数据，使用查询方式接收
** input parameters:    无
** output parameters:   无
** Returned value:      ucRcvData:   接收到的数据
*********************************************************************************************************/
uint8_t uartGetByte (void)
{
    uint8_t ucRcvData;
    
    while ((LPC_UART->LSR & 0x01) == 0);                                /* 等待接收标志置位             */
    ucRcvData = LPC_UART->RBR;                                          /* 读取数据                     */
    return (ucRcvData);   
}

/*********************************************************************************************************
** Function name:       uartGetStr
** Descriptions:        串口接收字符串
** input parameters:    pucStr:  指向接收数据数组的指针
**                      ulNum:   接收数据的个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartGetStr (uint8_t *pucStr, uint32_t ulNum)
{
    for (; ulNum > 0; ulNum--){
        *pucStr++ =  uartGetByte ();
    }  
}

/*********************************************************************************************************
** Function name:        uartSendByte
** Descriptions:        向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartSendByte (uint8_t ucDat)
{
    LPC_UART->THR = ucDat;                                              /*  写入数据                    */
    while ((LPC_UART->LSR & 0x40) == 0);                                /*  等待数据发送完毕            */
}

/*********************************************************************************************************
** Function name:        uartSendStr
** Descriptions:        向串口发送字符串
** input parameters:    puiStr:   要发送的字符串指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartSendStr (uint8_t  const *pucStr, uint32_t ulNum)
{
    while (ulNum-- > 0){
        uartSendByte (*pucStr++);
    }
}
