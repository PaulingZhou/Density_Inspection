#include "LPC11xx.h"                                                    /* LPC11xx����Ĵ���            */
#include "init.h"                                                       /* ��Ҫ����myDelay()���� */
#include "sht10.h"

/*********************************************************************************************************
�궨��
*********************************************************************************************************/
// #define DATAHIGH            (1ul << 7)           //DATA����ΪPIO2_7,DATABHIGH��ʾ�������߸ߵ�ƽ
// #define SCKHIGH             (1ul << 8)           //SCKΪ����PIO2_8,SCKHIGH��ʾʱ���߸ߵ�ƽ
// #define PIO2_7_FUNC         0x00                 //��PIO2_7����ΪGPIO����
// #define PIO2_8_FUNC         0x00                 //��PIO2_8����ΪGPIO����
// #define PIO2_MODE           0x02                 //��PIO2_7,PIO2_8����Ϊ�ڲ�����ʹ�ܣ���ʵ���ⲿ�����5V��10k����������ܱ�֤���ת����ʱ����������Եĵ�ƽ�½�

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
     myDelay(1);
     return error;
}

/*********************************************************************************************************
** Function name:       s_read_byte
** Descriptions:        ��Sensibus�϶�һ���ֽڲ�������Ӧ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
char s_read_byte(uint8_t ack)
{
    uint8_t i,val=0;
    LPC_GPIO2->DATA |= DATAHIGH;            //�ͷ�DATA������(��P2_7��������ģʽ������Ϊ����״̬���ע��)
    LPC_GPIO2->DIR &= ~DATAHIGH;            //�л�Ϊ����ģʽ
    for(i=0x80;i>0;i/=2)                    //�ܹ�8bit���ȶ���λ
    {
        LPC_GPIO2->DATA |= SCKHIGH;        //SCK�ߵ�ƽ����ʱ���Զ�ȡ����
        myDelay(1);                         //��ʱ1ms
        if(LPC_GPIO2->DATA&DATAHIGH) val = (val|i);            //��DATA�ߵ�ƽ��˵����λΪ1�����ģʽ���ȶ���λ
        LPC_GPIO2->DATA &= ~SCKHIGH;        //SCK�͵�ƽ����ʱsht10�ı�DATA��ƽ
        myDelay(1);                         // ��ʱ1ms
    }
    LPC_GPIO2->DIR |= DATAHIGH;             //DATA�л�Ϊ���ģʽ
    switch(ack)
    {
        case ACK:                           //��ΪACK
            LPC_GPIO2->DATA &= ~DATAHIGH;   //DATA�͵�ƽ������ȷ�ϸ��ֽ�
            break;
        case noACK:                         //��ΪnoACK
            LPC_GPIO2->DATA |= DATAHIGH;    //ackλDATA�ߵ�ƽ����ʹ��CRC-8У�飬��ֹͨѶ
            break;  
    }
    LPC_GPIO2->DATA |= SCKHIGH;             //CLK�ߵ�ƽ��sht10��ȡackλ
    myDelay(1);                             //��ʱ1ms
    LPC_GPIO2->DATA &= ~SCKHIGH;            //CLK�͵�ƽ��֮��������ͷ�DATA
    LPC_GPIO2->DATA |= DATAHIGH;            //DATA�ߵ�ƽ���������ͷ�DATA�����ڸ���״̬��ע��
    LPC_GPIO2->DIR &= ~DATAHIGH;            //DATA�л�Ϊ����ģʽ
    myDelay(1);
    return val;                             //���ز���ֵ(uint8_t���ͱ���)
}

/*********************************************************************************************************
** Function name:       s_connectionreset(void)
** Descriptions:        ͨѶ��λʱ��
** input parameters:    ��
                           ________________________________________     _____
** output parameters: DATA:                                        |___|
                             _   _   _   _   _   _   _   _   _     _   _
                      SCK: _| |_| |_| |_| |_| |_| |_| |_| |_| |___| |_| |____
** Returned value:      ��
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
** Descriptions:        �����λ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:        ������ʪ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
