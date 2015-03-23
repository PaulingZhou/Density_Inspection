#include "LPC11xx.h" 

/*********************************************************************************************************
** Function name:       uartGetByte
** Descriptions:        �Ӵ��ڽ���1�ֽ����ݣ�ʹ�ò�ѯ��ʽ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ucRcvData:   ���յ�������
*********************************************************************************************************/
uint8_t uartGetByte (void)
{
    uint8_t ucRcvData;
    
    while ((LPC_UART->LSR & 0x01) == 0);                                /* �ȴ����ձ�־��λ             */
    ucRcvData = LPC_UART->RBR;                                          /* ��ȡ����                     */
    return (ucRcvData);   
}

/*********************************************************************************************************
** Function name:       uartGetStr
** Descriptions:        ���ڽ����ַ���
** input parameters:    pucStr:  ָ��������������ָ��
**                      ulNum:   �������ݵĸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartGetStr (uint8_t *pucStr, uint32_t ulNum)
{
    for (; ulNum > 0; ulNum--){
        *pucStr++ =  uartGetByte ();
    }  
}

/*********************************************************************************************************
** Function name:        uartSendByte
** Descriptions:        �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartSendByte (uint8_t ucDat)
{
    LPC_UART->THR = ucDat;                                              /*  д������                    */
    while ((LPC_UART->LSR & 0x40) == 0);                                /*  �ȴ����ݷ������            */
}

/*********************************************************************************************************
** Function name:        uartSendStr
** Descriptions:        �򴮿ڷ����ַ���
** input parameters:    puiStr:   Ҫ���͵��ַ���ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartSendStr (uint8_t  const *pucStr, uint32_t ulNum)
{
    while (ulNum-- > 0){
        uartSendByte (*pucStr++);
    }
}
