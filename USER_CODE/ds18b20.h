#define ds_DATAGIGH            (1ul << 11)         //DATA����ΪPIO2_11,DATAHIGH��ʾ�������߸ߵ�ƽ
#define PIO2_11_FUNC        0x00                //��PIO2_11����ΪGPIOģʽ
#define PIO2_11_MODE        0x02                //��PIO2_11����Ϊ�ڲ�����ʹ��

extern void ds18b20Init(void);
extern void DS18B20_Reset(void);
extern void ds_write_byte(uint8_t value);
