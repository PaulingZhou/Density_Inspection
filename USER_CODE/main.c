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
    ** Descriptions:        ����Ũ�ȼ�
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
    #include "LPC11xx.h"                                                    /* LPC11xx����Ĵ���            */

    /*********************************************************************************************************
      �궨��
    *********************************************************************************************************/
    #define DATAHIGH            (1ul << 7)           //DATA����ΪPIO2_7,DATABHIGH��ʾ�������߸ߵ�ƽ
    #define SCKHIGH             (1ul << 8)           //SCKΪ����PIO2_8,SCKHIGH��ʾʱ���߸ߵ�ƽ
    #define PIO2_7_FUNC         0x00                 //��PIO2_7����ΪGPIO����
    #define PIO2_8_FUNC         0x00                 //��PIO2_8����ΪGPIO����
    #define PIO2_MODE           0x02                 //��PIO2_7,PIO2_8����Ϊ�ڲ�����ʹ��

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
    ** Descriptions:        �����ʱ
    ** input parameters:    ��
    ** output parameters:   ��
    ** Returned value:      ��
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
    ** Descriptions:        �������俪ʼָ��
    ** input parameters:    ��
    ** output parameters:         __      _____
                            DATA:   |____|
                                   __    __
                            SCK: _|  |__|  |___
    ** Returned value:      ��
    *********************************************************************************************************/
    void s_transstart(void){
            LPC_GPIO2->DIR  |= DATAHIGH;                              /* ��p2.7,p2.8����Ĵ�����1(����Ϊ���) */
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
            LPC_GPIO2->DIR  &= ~DATAHIGH;                              /* ��p2.7,p2.8����Ĵ�����0(����Ϊ����) */
    }



    /*********************************************************************************************************
    ** Function name:       GPIOInit
    ** Descriptions:        GPIO��ʼ��
    ** input parameters:    ��
    ** output parameters:   ��
    ** Returned value:      ��
    *********************************************************************************************************/
    void GPIOInit( void )
    {
        LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* ��ʼ��GPIO AHBʱ��       */
        LPC_IOCON->PIO2_7 = PIO2_7_FUNC|(PIO2_MODE<<3);                     /* ��P2.7����ΪGPIO���ܣ���������ʹ��      */
        LPC_IOCON->PIO2_8 &= PIO2_8_FUNC|(PIO2_MODE<<3);                    /* ��p2.8����ΪGPIO���ܣ���������ʹ��       */
        LPC_GPIO2->DIR  |= (DATAHIGH|SCKHIGH);                              /* ��p2.7,p2.8����Ĵ�����1(����Ϊ���) */
        LPC_GPIO2->DATA |= (DATAHIGH|SCKHIGH);                              /* ��p2.7,p2.8����Ϊ��ʼ������ߵ�ƽ */          
    }

    /*********************************************************************************************************
    ** Function name:       main
    ** Descriptions:        ��
    ** input parameters:    ��
    ** output parameters:   ��
    ** Returned value:      ��
    *********************************************************************************************************/
    int main (void)
    {
        SystemInit();                                                       /* ϵͳ��ʼ��������ɾ��         */
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
