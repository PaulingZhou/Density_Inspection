extern void myDelay (uint32_t ulTime);
#define DATAHIGH            (1ul << 7)           //DATA总线为PIO2_7,DATABHIGH表示数据总线高电平
#define SCKHIGH             (1ul << 8)           //SCK为总线PIO2_8,SCKHIGH表示时钟线高电平
#define PIO2_7_FUNC         0x00                 //把PIO2_7设置为GPIO功能
#define PIO2_8_FUNC         0x00                 //把PIO2_8设置为GPIO功能
#define PIO2_MODE           0x02                 //把PIO2_7,PIO2_8设置为内部上拉使能，但实测外部仍需接5V与10k上拉电阻才能保证输出转输入时不会产生明显的电平下降
