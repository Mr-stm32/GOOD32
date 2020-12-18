/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部 
**                                  http://www.maiwei.com.cn   
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_adc.c 
** 内容描述 : 
**		  					
**------------------------------------------------------------------------------------------------------
** 创 建 者 : Jason
** 版    本 : V1.0
** 创建日期 : 2020-12-12 
**------------------------------------------------------------------------------------------------------
** 修 改 者 : None	
** 版    本 : None	
** 修改日期 : None	
** 内容描述 : None	
********************************************************************************************************/
#include "hal_adc.h"

/*********************************************配置区****************************************************/

/*********************************************变量区****************************************************/


/*********************************************声明区****************************************************/


/******************************************内部函数区****************************************************/

/********************************************接口区******************************************************/
  //要实现
void fADC_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE );              // 使能 ADC1 通道时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                   // 设置 ADC 分频因子 6//72M/6=12,ADC 最大时间不能超过 14M
                    
//--GPIO_Init           
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;                 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;          
	GPIO_Init(GPIOC,&GPIO_InitStructure);           
//--ADC_Init            
            
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // 独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	                    // 连续多通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 不受外界决定
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                            // 扫描通道数1
	ADC_Init(ADC1,&ADC_InitStructure);
											
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);//ADC1,
	ADC_Cmd(ADC1,ENABLE);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

uint16_t fGetADC(void)
{
	uint8_t i=0;	
	uint32_t value=0;
	uint32_t conresult[16]={0};

//--循环读取16次
	for(i=0;i<16;)
	{
		if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==SET)
		{
			conresult[i] = (u32)ADC_GetConversionValue(ADC1);
			//2(12)=4096  3.3v=3300mv
			conresult[i] = conresult[i] * 3300;
			conresult[i] = conresult[i] / 4096;		
			value += conresult[i];
			i++;
		}
		ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
	}
	return((u16)(value/16));		
}


/****************************************END OF FILE****************************************************/
