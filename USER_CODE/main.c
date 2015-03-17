    /****************************************Copyright (c)****************************************************
    **                                          
    **
    **
    **--------------File Info---------------------------------------------------------------------------------
    ** File name:           main.c
    ** Last modified Date:  2015-3-15
    ** Last Version:        V1.0
    ** Descriptions:        The main() function 
    **
    **--------------------------------------------------------------------------------------------------------
    ** Created by:          Zhou Sihang
    ** Created date:        2015-3-15
    ** Version:             V1.00
    ** Descriptions:        气体浓度计
    **
    **--------------------------------------------------------------------------------------------------------
    ** Modified by:         
    ** Modified date:       
    ** Version:             
    ** Descriptions:        
    **
    **--------------------------------------------------------------------------------------------------------
    ** Modified by:        
    ** Modified date:      
    ** Version:            
    ** Descriptions:       
    **
    ** Rechecked by:
    *********************************************************************************************************/
    #include "LPC11xx.h"                                                    /* LPC11xx外设寄存器            */

    /*********************************************************************************************************
      宏定义
    *********************************************************************************************************/
    #define DATAHIGH            (1ul << 7)           //DATA总线为PIO2_7,DATABHIGH表示数据总线高电平
    #define SCKHIGH             (1ul << 8)           //SCK为总线PIO2_8,SCKHIGH表示时钟线高电平
    #define PIO2_7_FUNC         0x00                 //把PIO2_7设置为GPIO功能
    #define PIO2_8_FUNC         0x00                 //把PIO2_8设置为GPIO功能
    #define PIO2_MODE           0x02                 //把PIO2_7,PIO2_8设置为内部上拉使能

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
    ** Descriptions:        从Sensibus上写一个字节并检查是否有回应
    ** input parameters:    无
    ** output parameters:   无
    ** Returned value:      无
    *********************************************************************************************************/
    // char s_write_byte(uint8_t value)
    // {
    //     uint8_t i,error = 0;
    //     for(i=0x80;i>0;i/=2){                   //shift bit for masking,0x80=1000 0000
    //         if (i & value) LPC_GPIO0->DATA |= DATAHIGH;              //masking value with i,write to SENSI-BUS
    //         else LPC_GPIO0->DATA &= ~DATAHIGH;;
    //         LPC_GPIO0->DATA |= SCKHIGH;
    //         for(i=0;i<3;i++);
    //         LPC_GPIO0->DATA &= ~SCKHIGH;
    //     }
    //     LPC_GPIO0->DATA |= DATAHIGH;
    //     LPC_GPIO0->DATA |= SCKHIGH;
    //     error=DATAHIGH;
    //     LPC_GPIO0->DATA &= ~SCKHIGH;
    //     return error;
    // }
    
        /*********************************************************************************************************
    ** Function name:       myDelay
    ** Descriptions:        软件延时
    ** input parameters:    无
    ** output parameters:   无
    ** Returned value:      无
    *********************************************************************************************************/
    void myDelay (uint32_t ulTime)
    {
        uint32_t i;

        i = 0;
        while (ulTime--) {
            for (i = 0; i < 5000; i++);
        }
    }

    /*********************************************************************************************************
    ** Function name:       s_transstart
    ** Descriptions:        产生传输开始指令
    ** input parameters:    无
    ** output parameters:         __      _____
                            DATA:   |____|
                                   __    __
                            SCK: _|  |__|  |___
    ** Returned value:      无
    *********************************************************************************************************/
    void s_transstart(void){
            LPC_GPIO2->DIR  |= DATAHIGH;                              /* 将p2.7,p2.8方向寄存器置1(配置为输出) */
            LPC_GPIO2->DATA |= DATAHIGH;        //DATA=1
            LPC_GPIO2->DATA &= ~SCKHIGH;        //SCK=0
            __nop();
//             myDelay(10);
            LPC_GPIO2->DATA |= SCKHIGH;         //SCK=1
            __nop();
//             myDelay(10);
            LPC_GPIO2->DATA &= ~DATAHIGH;       //DATA=0
//             myDelay(10);
            __nop();
            LPC_GPIO2->DATA &= ~SCKHIGH;        //SCK=0
//             myDelay(30);
            __nop();__nop();__nop();
            LPC_GPIO2->DATA |= SCKHIGH;         //SCK=1
//             myDelay(10);
            __nop();
            LPC_GPIO2->DATA |= DATAHIGH;        //DATA=1
//             myDelay(10);
            __nop();
            LPC_GPIO2->DATA &= ~SCKHIGH;        //SCK=0
//             myDelay(10);
            __nop();
            LPC_GPIO2->DIR  &= ~DATAHIGH;                              /* 将p2.7,p2.8方向寄存器置0(配置为输入) */
    }



    /*********************************************************************************************************
    ** Function name:       GPIOInit
    ** Descriptions:        GPIO初始化
    ** input parameters:    无
    ** output parameters:   无
    ** Returned value:      无
    *********************************************************************************************************/
    void GPIOInit( void )
    {
        LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* 初始化GPIO AHB时钟       */
        LPC_IOCON->PIO2_7 = PIO2_7_FUNC|(PIO2_MODE<<3);                     /* 将P2.7定义为GPIO功能，上拉电阻使能      */
        LPC_IOCON->PIO2_8 &= PIO2_8_FUNC|(PIO2_MODE<<3);                    /* 将p2.8定义为GPIO功能，上拉电阻使能       */
        LPC_GPIO2->DIR  |= (DATAHIGH|SCKHIGH);                              /* 将p2.7,p2.8方向寄存器置1(配置为输出) */
        LPC_GPIO2->DATA |= (DATAHIGH|SCKHIGH);                              /* 将p2.7,p2.8配置为初始化输出高电平 */          
    }

    /*********************************************************************************************************
    ** Function name:       main
    ** Descriptions:        无
    ** input parameters:    无
    ** output parameters:   无
    ** Returned value:      无
    *********************************************************************************************************/
    int main (void)
    {
        SystemInit();                                                       /* 系统初始化，切勿删除         */
        GPIOInit();
        while (1) {
            s_transstart();
//             myDelay(1000);
            __nop();__nop();__nop();__nop;__nop();__nop();
        }
    }

    /*********************************************************************************************************
      End Of File
    *********************************************************************************************************/
