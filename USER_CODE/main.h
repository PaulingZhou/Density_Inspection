extern void myDelay (uint32_t ulTime);
#define DATAHIGH            (1ul << 7)           //DATA����ΪPIO2_7,DATABHIGH��ʾ�������߸ߵ�ƽ
#define SCKHIGH             (1ul << 8)           //SCKΪ����PIO2_8,SCKHIGH��ʾʱ���߸ߵ�ƽ
#define PIO2_7_FUNC         0x00                 //��PIO2_7����ΪGPIO����
#define PIO2_8_FUNC         0x00                 //��PIO2_8����ΪGPIO����
#define PIO2_MODE           0x02                 //��PIO2_7,PIO2_8����Ϊ�ڲ�����ʹ�ܣ���ʵ���ⲿ�����5V��10k����������ܱ�֤���ת����ʱ����������Եĵ�ƽ�½�
