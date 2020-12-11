/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部 
**                                  http://www.maiwei.com.cn   
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_pwm.c 
** 内容描述 : 
**		  					
**------------------------------------------------------------------------------------------------------
** 创 建 者 : Jason
** 版    本 : V1.0
** 创建日期 : 2020-12-09 
**------------------------------------------------------------------------------------------------------
** 修 改 者 : None	
** 版    本 : None	
** 修改日期 : None	
** 内容描述 : None	
********************************************************************************************************/
#include "hal_pwm.h"


/*********************************************************************************************************
** 函数名称 ：TIM3_PWM_Init
** 函数功能 ：
** 入口参数 ：arr: 
**			  psc: 
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);   // 使能 GPIO 和 AFIO 复用功能时钟
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);                           // 重映射 TIM3_CH2->PC7
    
    //设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 GPIOC.7
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;                              
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;                              
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
    //初始化 TIM3
    TIM_TimeBaseStructure.TIM_Period        = arr;             
    TIM_TimeBaseStructure.TIM_Prescaler     = psc; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;               // TIM 向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                             // 初始化 TIMx
    //初始化 TIM3 Channel2 PWM 模式
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;                      // 选择 PWM 模式 2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               // 比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;                  // 输出极性高
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                                    // 初始化外设 TIM3 OC1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);                           // 使能预装载寄存器
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                                    // 初始化外设 TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);                           // 使能预装载寄存器
    TIM_Cmd(TIM3, ENABLE);                                                      // 使能 TIM3
}

/****************************************END OF FILE****************************************************/
