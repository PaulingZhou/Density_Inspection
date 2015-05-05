#define ds_DATAGIGH            (1ul << 11)         //DATA总线为PIO2_11,DATAHIGH表示数据总线高电平
#define PIO2_11_FUNC        0x00                //将PIO2_11设置为GPIO模式
#define PIO2_11_MODE        0x02                //将PIO2_11设置为内部上拉使能

extern void ds18b20Init(void);
extern void DS18B20_Reset(void);
extern void ds_write_byte(uint8_t value);
