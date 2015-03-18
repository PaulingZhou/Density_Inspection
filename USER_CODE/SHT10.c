#include "LPC11xx.h"                                                    /* LPC11xx����Ĵ���            */
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
     LPC_GPIO2->DATA &= ~SCKHIGH;                               //SCK�͵�ƽ
     return error;
}

    
/*********************************************************************************************************
** Function name:       s_transstart
** Descriptions:        �������俪ʼָ��
** input parameters:    ��
** output parameters:       __      _____
                      DATA:   |____|
                             __    __
                      SCK: _|  |__|  |___
** Returned value:      ��
*********************************************************************************************************/
void s_transstart(void)
{
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
