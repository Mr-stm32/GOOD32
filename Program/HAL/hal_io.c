/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部 
**                                  http://www.maiwei.com.cn   
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_io.c 
** 内容描述 : stm32f103vet6平台下GPIO初始化。
**		  					
**------------------------------------------------------------------------------------------------------
** 创 建 者 : Jason
** 版    本 : V1.0
** 创建日期 : 2020-12-08 
**------------------------------------------------------------------------------------------------------
** 修 改 者 : None	
** 版    本 : None	
** 修改日期 : None	
** 内容描述 : None	
********************************************************************************************************/
#include "hal_io.h"

#if (DEF_STM32F10X_GPIO_EN > 0)
/*********************************************配置区****************************************************/
#define	DEF_EXIT_IRQ_MAXNUM	  7 // 外部中断组个数0~4、9-5、15-10一共7个


/*********************************************变量区****************************************************/



/*********************************************声明区****************************************************/

static fEXIT_CallFunction EXIT_IRQHandler[DEF_EXIT_IRQ_MAXNUM]={0};

#endif //DEF_STM32F10X_GPIO_EN
/******************************************内部函数区****************************************************/


/******************************************接口函数区****************************************************/
/*********************************************************************************************************
** 函数名称 ：fGPIO_Init
** 函数功能 ：初始化IO
** 入口参数 ：GPIOx: IO组
**			  GPIO_Pin: IO号
**			  GPIO_Mode: IO模式
**                       GPIO_Mode_AIN;
**                       GPIO_Mode_IN_FLOATING;
**                       GPIO_Mode_IPD ;
**                       GPIO_Mode_IPU;
**                       GPIO_Mode_Out_OD;
**                       GPIO_Mode_Out_PP;
**                       GPIO_Mode_AF_OD;
**                       GPIO_Mode_AF_PP; 
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fGPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode)
{
#if (DEF_STM32F10X_GPIO_EN > 0)	
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                         | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD 
                         | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);  
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin;		
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：fGPIO_NVIC_Init
** 函数功能 ：外部中断向量配置
** 入口参数 ：irqChannel: 中断源
**                        EXTI0_IRQn~EXTI4_IRQn/EXTI9_5_IRQn/EXTI15_10_IRQn
**			  preemption: 抢占优先级
**			  sub: 子优先级
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fGPIO_NVIC_Init(uint8_t irqChannel, uint8_t preemption, uint8_t sub)
{
#if (DEF_STM32F10X_GPIO_EN > 0)	
    
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irqChannel;                    // 配置中断源

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption; // 配置抢占优先级

    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub;               // 配置子优先级

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                    // 使能中断通道

    NVIC_Init(&NVIC_InitStructure);
    
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：fGPIO_EXTI_Init
** 函数功能 ：配置外部中断信道
** 入口参数 ：GPIO_PortSource: GPIO_PortSourceGPIOA~GPIO_PortSourceGPIOF      
**            GPIO_PinSource : GPIO_PinSource0~GPIO_PinSource15
**            EXTI_Line      : EXTI_Line0~EXTI_Line15
**			  EXTI_Trigger   : EXTI_Trigger_Rising;
**                             EXTI_Trigger_Falling;  
**                             EXTI_Trigger_Rising_Falling;
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fGPIO_EXTI_Init(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource, uint32_t EXTI_Line, EXTITrigger_TypeDef EXTI_Trigger)
{
#if (DEF_STM32F10X_GPIO_EN > 0)
    
    EXTI_InitTypeDef EXTI_InitStructure;
    
    GPIO_EXTILineConfig(GPIO_PortSource,GPIO_PinSource);    
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line;

    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;// EXTI 为外部中断模式

    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger;    // 下降沿中断

    EXTI_InitStructure.EXTI_LineCmd = ENABLE;       
    
    EXTI_Init(&EXTI_InitStructure);
    
    EXTI_ClearITPendingBit(EXTI_Line);
    
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：fSetGPIO_EXTI_CallBack
** 函数功能 ：设置外部中断0~4、9-5、15-10中断执行函数
** 入口参数 ：CallBackfunc: 中断执行函数指针
**			  channel: 外部中断号(0-15)
** 出口参数 ：OK/ERR_CHANNEL/ERR_EXIST
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fSetGPIO_EXTI_CallBack(fEXIT_CallFunction CallBackfunc, uint8_t channel)
{
#if (DEF_STM32F10X_GPIO_EN > 0)
    
    #define OK             0 
    #define ERR_CHANNEL    1 
    #define ERR_EXIST      2 
    
    if(channel >= DEF_EXIT_IRQ_MAXNUM)
    {
        return ERR_CHANNEL;
    }

    if(EXIT_IRQHandler[channel] !=0)
    {
        return ERR_EXIST;  
    }
    
    EXIT_IRQHandler[channel]=CallBackfunc;	
 
	return OK;
#else
    return 0;       // 返回0，相当于成功.防止因为关闭某一个功能，使得整个程序不能继续运行
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：EXTI0_IRQHandler
** 函数功能 ：外部中断
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void EXTI0_IRQHandler(void)
{
#if (DEF_STM32F10X_GPIO_EN > 0)
    
    if(EXIT_IRQHandler[0] != 0)
    {
        EXIT_IRQHandler[0](0); 
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
    
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：EXTI1_IRQHandler
** 函数功能 ：外部中断
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void EXTI1_IRQHandler(void)
{
#if (DEF_STM32F10X_GPIO_EN > 0)
    
    if(EXIT_IRQHandler[1] != 0)
    {
        EXIT_IRQHandler[1](1); 
    }    
    EXTI_ClearITPendingBit(EXTI_Line1);
    
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：EXTI2_IRQHandler
** 函数功能 ：外部中断
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void EXTI2_IRQHandler(void)
{
#if (DEF_STM32F10X_GPIO_EN > 0)
    
    if(EXIT_IRQHandler[2] != 0)
    {
        EXIT_IRQHandler[2](2); 
    }    
    EXTI_ClearITPendingBit(EXTI_Line2);
    
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：EXTI3_IRQHandler
** 函数功能 ：外部中断
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void EXTI3_IRQHandler(void)
{
#if (DEF_STM32F10X_GPIO_EN > 0)
      
    if(EXIT_IRQHandler[3] != 0)
    {
        EXIT_IRQHandler[3](3); 
    }    
    EXTI_ClearITPendingBit(EXTI_Line3);
    
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：EXTI4_IRQHandler
** 函数功能 ：外部中断
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void EXTI4_IRQHandler(void)
{
#if (DEF_STM32F10X_GPIO_EN > 0)
      
    if(EXIT_IRQHandler[4] != 0)
    {
        EXIT_IRQHandler[4](4); 
    }    
    EXTI_ClearITPendingBit(EXTI_Line4);
    
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：EXTI9_5_IRQHandler
** 函数功能 ：外部中断
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void EXTI9_5_IRQHandler(void)
{   
#if (DEF_STM32F10X_GPIO_EN > 0)
      
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)  
    {
        if(EXIT_IRQHandler[5] != 0)
        {
            EXIT_IRQHandler[5](5); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line5);
    }    
    
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)  
    {
        if(EXIT_IRQHandler[5] != 0)
        {
            EXIT_IRQHandler[5](6); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line6);
    }  
    
    if (EXTI_GetITStatus(EXTI_Line7) != RESET)  
    {
        if(EXIT_IRQHandler[5] != 0)
        {
            EXIT_IRQHandler[5](7); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line7);
    } 
    
    if (EXTI_GetITStatus(EXTI_Line8) != RESET)  
    {
        if(EXIT_IRQHandler[5] != 0)
        {
            EXIT_IRQHandler[5](8); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line8);
    } 
    
    if (EXTI_GetITStatus(EXTI_Line9) != RESET)  
    {
        if(EXIT_IRQHandler[5] != 0)
        {
            EXIT_IRQHandler[5](9); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line9);
    }  
    
#endif //DEF_STM32F10X_GPIO_EN
}

/*********************************************************************************************************
** 函数名称 ：EXTI15_10_IRQHandler
** 函数功能 ：外部中断
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void EXTI15_10_IRQHandler(void)
{
#if (DEF_STM32F10X_GPIO_EN > 0)
      
    if (EXTI_GetITStatus(EXTI_Line10) != RESET)  
    {
        if(EXIT_IRQHandler[6] != 0)
        {
            EXIT_IRQHandler[6](10); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line10);
    }    
    
    if (EXTI_GetITStatus(EXTI_Line11) != RESET)  
    {
        if(EXIT_IRQHandler[6] != 0)
        {
            EXIT_IRQHandler[6](11); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line11);
    }  
    
    if (EXTI_GetITStatus(EXTI_Line12) != RESET)  
    {
        if(EXIT_IRQHandler[6] != 0)
        {
            EXIT_IRQHandler[6](12); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line12);
    } 
    
    if (EXTI_GetITStatus(EXTI_Line13) != RESET)  
    {
        if(EXIT_IRQHandler[6] != 0)
        {
            EXIT_IRQHandler[6](13); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line13);
    } 
    
    if (EXTI_GetITStatus(EXTI_Line14) != RESET)  
    {
        if(EXIT_IRQHandler[6] != 0)
        {
            EXIT_IRQHandler[6](14); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line14);
    } 
    if (EXTI_GetITStatus(EXTI_Line15) != RESET)  
    {
        if(EXIT_IRQHandler[6] != 0)
        {
            EXIT_IRQHandler[6](15); 
        } 
        EXTI_ClearITPendingBit(EXTI_Line15);
    }  
    
#endif //DEF_STM32F10X_GPIO_EN
}

/****************************************END OF FILE****************************************************/
