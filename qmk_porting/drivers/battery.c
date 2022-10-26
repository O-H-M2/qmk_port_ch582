    void battery_init()
    {
        GPIOA_ModeCfg( GPIO_Pin_2, GPIO_ModeIN_Floating );
        ADC_ExtSingleChSampInit( SampleFreq_3_2, ADC_PGA_2 );
    }
      
    UINT32 battery_read()
    {
        UINT32 adc_data;
        UINT32 adc_volt;
        UINT16 abcBuff[15];
        GPIOA_ModeCfg( GPIO_Pin_5, GPIO_ModeIN_Floating );
        RoughCalib_Value = ADC_DataCalib_Rough(); // 用于计算ADC内部偏差，记录到全局变量 RoughCalib_Value中

        ADC_ChannelCfg( 7 );//对应PA2

        for( i = 0; i < 15; i++ )
        {
            abcBuff[i] = ADC_ExcutSingleConver() + RoughCalib_Value;      // 连续采样15次 使用后10次
        }

        PRINT( "%d \n", abcBuff[10] );

        adc_data = 0;
        for( i = 0; i < 10; i++ )
        {
            adc_data += abcBuff[i+5] ;      // 丢弃前5次采样
        }
        adc_volt =  (adc_data *10 /4096 +0.5*100 )*105*3;//数据放大1000倍 不计算小数

        return adc_volt
        //PRINT( "adc_volt ：%d \n", adc_volt );
    }