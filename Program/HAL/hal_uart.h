/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部 
**                                  http://www.maiwei.com.cn   
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_uart.h 
** 内容描述 : stm32f103vet6平台下UART初始化，数据发送以及接收。
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
#ifndef __HAL_UART_H_
#define __HAL_UART_H_
/*******************************************头文件区****************************************************/
#include "stm32f10x.h"

/*******************************************配置区******************************************************/
#define DEF_STM32F10X_UART_EN   1u

#if (DEF_STM32F10X_UART_EN > 0)
#define USART1_EN               1u
#define USART2_EN               1u
#define USART3_EN               1u
#define UART4_EN                1u
#define UART5_EN                1u
#endif // DEF_STM32F10X_UART_EN

#if (USART1_EN > 0)
	#define USART1_PORT								GPIOA								
	#define USART1_RX_PIN							GPIO_Pin_10							
	#define USART1_TX_PIN							GPIO_Pin_9						
	#define	USART1_BAUD_RATE						115200
	#define USART1_PREEMPT				0
	#define USART1_SUB						1
    #define USART1_IT_TX_START                      USART_ITConfig(USART1, USART_IT_TXE, ENABLE)
#endif // USART1_EN

#if (USART2_EN > 0)
	#define USART2_PORT								GPIOA								
	#define USART2_RX_PIN							GPIO_Pin_3							
	#define USART2_TX_PIN							GPIO_Pin_2					
	#define	USART2_BAUD_RATE						115200
	#define USART2_PREEMPT				            0
	#define USART2_SUB						        1
    #define USART2_IT_TX_START                      USART_ITConfig(USART2, USART_IT_TXE, ENABLE)
#endif // USART2_EN

#if (USART3_EN > 0)
	#define USART3_PORT								GPIOB								
	#define USART3_RX_PIN							GPIO_Pin_11						
	#define USART3_TX_PIN							GPIO_Pin_10					
	#define	USART3_BAUD_RATE						115200
	#define USART3_PREEMPT				            0
	#define USART3_SUB						        2
    #define USART3_IT_TX_START                      USART_ITConfig(USART3, USART_IT_TXE, ENABLE)
#endif // USART3_EN

#if (UART4_EN > 0)
	#define UART4_PORT								GPIOC								
	#define UART4_RX_PIN							GPIO_Pin_11						
	#define UART4_TX_PIN							GPIO_Pin_10					
	#define	UART4_BAUD_RATE						    115200
	#define UART4_PREEMPT				            0
	#define UART4_SUB						        3
    #define UART4_IT_TX_START                       USART_ITConfig(UART4, USART_IT_TXE, ENABLE)
#endif // UART4_EN

#if (UART5_EN > 0)
	#define UART5_RX_PORT						    GPIOD
	#define UART5_TX_PORT						    GPIOC
	#define UART5_RX_PIN							GPIO_Pin_2					
	#define UART5_TX_PIN							GPIO_Pin_12					
	#define	UART5_BAUD_RATE						    115200
	#define UART5_PREEMPT				            0
	#define UART5_SUB						        4
    #define UART5_IT_TX_START                       USART_ITConfig(UART5, USART_IT_TXE, ENABLE)
#endif // UART5_EN

/*******************************************自定义类型区************************************************/
typedef enum { 
	MY_UART1 = 0, 
	MY_UART2, 
	MY_UART3,
	MY_UART4,
	MY_UART5}E_UartName; // 串口号 

typedef uint8_t (*fUartGetSendDataCallFunc)(E_UartName, uint8_t); // 返回0成功；argv[0]:串口号；argv[1]:发送值
typedef void (*fUartPostRcvDataCallFunc)(E_UartName, uint8_t);    // argv[0]:串口号；argv[1]:接收值
typedef void (*fUartSendEndHandlerCallFunc)(E_UartName);          // argv[0]:串口号；
    
/*******************************************接口区*******************************************************/
/*********************************************************************************************************
** 函数名称 ：fUartSetCallFunction
** 函数功能 ：设置串口回调函数
** 入口参数 ：fGetSendDataFunc: 串口中断发送数据时，获取将发的数值；
**			  fPostRcvDataFunc: 串口中断接收数据时，将数据上报。
**			  fSendEndHandlerFunc: 串口中断发送结束后执行操作。
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-10
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
extern void fUartSetCallFunction(fUartGetSendDataCallFunc fGetSendDataFunc, fUartPostRcvDataCallFunc fPostRcvDataFunc, fUartSendEndHandlerCallFunc fSendEndHandlerFunc);
/*********************************************************************************************************
** 函数名称 ：fUSART1_Init
** 函数功能 ：stm32f103串口1初始化
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
extern void fUSART1_Init(void);
/*********************************************************************************************************
** 函数名称 ：fUSART1_Init
** 函数功能 ：stm32f103串口1初始化
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
extern void fUSART2_Init(void);
/*********************************************************************************************************
** 函数名称 ：fUSART3_Init
** 函数功能 ：stm32f103串口3初始化
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
extern void fUSART3_Init(void);
/*********************************************************************************************************
** 函数名称 ：fUART4_Init
** 函数功能 ：stm32f103串口4初始化
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
extern void fUART4_Init(void);
/*********************************************************************************************************
** 函数名称 ：fUART5_Init
** 函数功能 ：stm32f103串口5初始化
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
extern void fUART5_Init(void);
/*********************************************************************************************************
** 函数名称 ：fUSART_SendArrayData
** 函数功能 ：串口等待式发送数据
** 入口参数 ：array: 发送的数组
**			  length: 数组长度
** 出口参数 ：成功:0; 失败:1;
** 作者日期 ：Jason 2020-12-10
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
extern uint8_t fUSART_SendArrayData(E_UartName nUart,const uint8_t *array,const uint32_t length);   

#endif //__HAL_UART_H_
/****************************************END OF FILE****************************************************/
