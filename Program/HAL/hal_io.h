/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部 
**                                  http://www.maiwei.com.cn   
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_io.h 
** 内容描述 : stm32f103vet6 io配置初始化。
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
#ifndef __HAL_IO_H_
#define __HAL_IO_H_
/*******************************************头文件区****************************************************/
#include "stm32f10x.h"

/*******************************************配置区******************************************************/
#define DEF_STM32F10X_GPIO_EN    0u

//--IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//--IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//--IO口操作,只对单一的IO口.确保n的值小于16
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

//--KEY
#define KEY1_PORT         	    GPIOE
#define KEY1_PIN         	    GPIO_Pin_5
#define KEY1_MODE               GPIO_Mode_IPD   
#define KEY1_IRQ_CHANNEL        EXTI9_5_IRQn
#define KEY1_IRQ_PREEMPT        1
#define KEY1_IRQ_SUB            1
#define KEY1_IRQ_PORTSOURCE     GPIO_PortSourceGPIOE      
#define KEY1_IRQ_PINSOURCE      GPIO_PinSource5 
#define KEY1_EXTI_LINE          EXTI_Line5
#define KEY1_EXTI_TRIGGER       EXTI_Trigger_Falling
#define	KEY1_VAL			    GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN)

#define KEY2_PORT         	    GPIOE
#define KEY2_PIN         	    GPIO_Pin_4
#define KEY2_MODE               GPIO_Mode_IPU     
#define	KEY2_VAL			    GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN)

#define KEY3_PORT         	    GPIOE
#define KEY3_PIN         	    GPIO_Pin_3
#define KEY3_MODE               GPIO_Mode_IPU     
#define	KEY3_VAL			    GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN)

#define KEY4_PORT         	    GPIOE
#define KEY4_PIN         	    GPIO_Pin_2
#define KEY4_MODE               GPIO_Mode_IPU     
#define	KEY4_VAL			    GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN)

//--LED
#define LED1_PORT         	    GPIOC
#define LED1_PIN         	    GPIO_Pin_6
#define LED1_MODE               GPIO_Mode_Out_PP               
#define	LED1_ON                 GPIO_SetBits(LED1_PORT, LED1_PIN)
#define	LED1_OFF                GPIO_ResetBits(LED1_PORT, LED1_PIN)

#define LED2_PORT         	    GPIOC
#define LED2_PIN         	    GPIO_Pin_7
#define LED2_MODE               GPIO_Mode_Out_PP     
#define	LED2_ON                 GPIO_SetBits(LED2_PORT, LED2_PIN)
#define	LED2_OFF                GPIO_ResetBits(LED2_PORT, LED2_PIN)

#define LED3_PORT         	    GPIOD
#define LED3_PIN         	    GPIO_Pin_13
#define LED3_MODE               GPIO_Mode_Out_PP     
#define	LED3_ON                 GPIO_SetBits(LED3_PORT, LED3_PIN)
#define	LED3_OFF                GPIO_ResetBits(LED3_PORT, LED3_PIN)    

#define LED4_PORT         	    GPIOD
#define LED4_PIN         	    GPIO_Pin_6
#define LED4_MODE               GPIO_Mode_Out_PP     
#define	LED4_ON                 GPIO_SetBits(LED4_PORT, LED4_PIN)
#define	LED4_OFF                GPIO_ResetBits(LED4_PORT, LED4_PIN)  

//--BUZZER
#define BUZZER_PORT         	GPIOE
#define BUZZER_PIN         	    GPIO_Pin_0
#define BUZZER_MODE             GPIO_Mode_Out_PP     
#define	BUZZER_ON               GPIO_SetBits(BUZZER_PORT, BUZZER_PIN)
#define	BUZZER_OFF              GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN)  

//--ENC28J60
#define ENC28J60_RST_PORT       GPIOA
#define ENC28J60_RST_PIN        GPIO_Pin_1
#define ENC28J60_MODE           GPIO_Mode_Out_PP     
#define	ENC28J60_RST            PAout(n)           



/*******************************************自定义类型区************************************************/
typedef void (*fEXIT_CallFunction)(uint8_t);

/*******************************************接口区*******************************************************/
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
extern void fGPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);
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
extern void fGPIO_NVIC_Init(uint8_t irqChannel, uint8_t preemption, uint8_t sub);
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
extern void fGPIO_EXTI_Init(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource, uint32_t EXTI_Line, EXTITrigger_TypeDef EXTI_Trigger);
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
extern uint8_t fSetGPIO_EXTI_CallBack(fEXIT_CallFunction CallBackfunc, uint8_t channel);
#endif //__HAL_IO_H_

/****************************************END OF FILE****************************************************/
