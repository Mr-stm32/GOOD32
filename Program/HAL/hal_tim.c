/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部
**                                  http://www.maiwei.com.cn
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_tim.c
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
#include "hal_tim.h"
/*********************************************配置区****************************************************/



/*********************************************变量区****************************************************/
static uint32_t delayMs = 0;


/*********************************************声明区****************************************************/




/******************************************内部函数区****************************************************/


/******************************************接口函数区****************************************************/
/*********************************************************************************************************
** 函数名称 ：fDelayMs
** 函数功能 ：通过定时器提供ms级延时函数
** 入口参数 ：nMs: 延时时间
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fDelayMs(uint32_t nMs)
{
    if(delayMs != 0)return;

    delayMs = nMs;
    while(delayMs);
}


/*********************************************************************************************************
** 函数名称 ：TIM6_IRQHandler
** 函数功能 ：定时器6中断函数
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void TIM6_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        if(delayMs)
        {
            delayMs --;
        }
    }
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );
}

/*********************************************************************************************************
** 函数名称 ：fTIM6_Init
** 函数功能 ：定时器6初始化
** 入口参数 ：ms:定时器中断时间
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fTIM6_Init(uint16_t ms)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 

    TIM_TimeBaseStructure.TIM_Period        = ms;     
    TIM_TimeBaseStructure.TIM_Prescaler     = 3600 - 1;   
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;     
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);     

    TIM_ITConfig(TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);

    TIM_Cmd(TIM6, ENABLE);  

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM6_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM6_IRQ_PREEMPT;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = TIM6_IRQ_SUB;  
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);  
}


/****************************************END OF FILE****************************************************/
