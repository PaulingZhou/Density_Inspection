extern void myDelay (uint32_t ulTime);
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
