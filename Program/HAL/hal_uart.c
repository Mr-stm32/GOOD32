/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部
**                                  http://www.maiwei.com.cn
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_uart.c
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
#include "hal_uart.h"

/*********************************************配置区****************************************************/


/*********************************************变量区****************************************************/
static fUartGetSendDataCallFunc fGetSendData = 0;
static fUartPostRcvDataCallFunc fPostRcvData = 0;
static fUartSendEndHandlerCallFunc fSendEndHandler = 0;

/*********************************************声明区****************************************************/

/******************************************内部函数区****************************************************/

/******************************************接口函数区****************************************************/
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
void fUartSetCallFunction(fUartGetSendDataCallFunc fGetSendDataFunc, fUartPostRcvDataCallFunc fPostRcvDataFunc, fUartSendEndHandlerCallFunc fSendEndHandlerFunc)
{
    fGetSendData = fGetSendDataFunc;
    fPostRcvData = fPostRcvDataFunc;
    fSendEndHandler = fSendEndHandlerFunc;
}

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
void fUSART1_Init(void)
{
#if (USART1_EN > 0)
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

//--配置串口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                           | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
                           | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

//--配置串口引脚
    GPIO_InitStructure.GPIO_Pin  = USART1_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(USART1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = USART1_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(USART1_PORT, &GPIO_InitStructure);

//--配置中断向量
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART1_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART1_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

//--配置串口参数
    USART_InitStructure.USART_BaudRate            = USART1_BAUD_RATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

    USART_ClearFlag(USART1, USART_FLAG_TC);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

#endif // USART1_EN
}

/*********************************************************************************************************
** 函数名称 ：USART1_IRQHandler
** 函数功能 ：串口1中断发送接收处理函数
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void USART1_IRQHandler(void)
{
#if (USART1_EN > 0)
    uint8_t data = 0;

//--处理接收的数据
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        data = (uint8_t)USART_ReceiveData(USART1);

        if(fPostRcvData != 0)
        {
            fPostRcvData(MY_UART1, data);                   // 将串口读取到的数据上传给使用者
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

//--处理发送的数据
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
    {
        if(fGetSendData != 0)
        {
            if(fGetSendData(MY_UART1, data) == 0)           // 获取要发送的数据
            {
                USART_SendData(USART1, data);
            }
            else
            {
                fSendEndHandler(MY_UART1);                  // 发送结束处理，为RS485等预留
            }
        }
        USART_ClearITPendingBit(USART1, USART_IT_TXE);
    }

#endif // USART1_EN    
}

/*********************************************************************************************************
** 函数名称 ：fUSART2_Init
** 函数功能 ：stm32f103串口2初始化
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fUSART2_Init(void)
{
#if (USART2_EN > 0)
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

//--配置串口时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                           | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
                           | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

//--配置中断向量
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART2_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

//--配置串口引脚
    GPIO_InitStructure.GPIO_Pin  = USART2_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(USART2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = USART2_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(USART2_PORT, &GPIO_InitStructure);

//--配置串口参数
    USART_InitStructure.USART_BaudRate            = USART2_BAUD_RATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);

    USART_Cmd(USART2, ENABLE);

    USART_ClearFlag(USART2, USART_FLAG_TC);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

#endif // USART2_EN
}

/*********************************************************************************************************
** 函数名称 ：USART2_IRQHandler
** 函数功能 ：串口2中断发送接收处理函数
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void USART2_IRQHandler(void)
{
#if (USART2_EN > 0)
    uint8_t data = 0;

//--处理接收的数据
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        data = (uint8_t)USART_ReceiveData(USART2);

        if(fPostRcvData != 0)
        {
            fPostRcvData(MY_UART2, data);                   // 将串口读取到的数据上传给使用者
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }

//--处理发送的数据
    if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
    {
        if(fGetSendData != 0)
        {
            if(fGetSendData(MY_UART2, data) == 0)           // 获取要发送的数据
            {
                USART_SendData(USART2, data);
            }
            else
            {
                USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
                fSendEndHandler(MY_UART2);                  // 发送结束处理，为RS485等预留
            }
        }
        USART_ClearITPendingBit(USART2, USART_IT_TXE);
    }

#endif // USART2_EN    
}

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
void fUSART3_Init(void)
{
#if (USART3_EN > 0)
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

//--配置串口时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                           | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
                           | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

//--配置中断向量
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART3_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART3_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

//--配置串口引脚
    GPIO_InitStructure.GPIO_Pin  = USART3_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(USART3_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = USART3_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(USART3_PORT, &GPIO_InitStructure);

//--配置串口参数
    USART_InitStructure.USART_BaudRate            = USART3_BAUD_RATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);

    USART_Cmd(USART3, ENABLE);

    USART_ClearFlag(USART3, USART_FLAG_TC);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

#endif // USART3_EN
}

/*********************************************************************************************************
** 函数名称 ：USART3_IRQHandler
** 函数功能 ：串口3中断发送接收处理函数
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void USART3_IRQHandler(void)
{
#if (USART3_EN > 0)
    uint8_t data = 0;

//--处理接收的数据
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        data = (uint8_t)USART_ReceiveData(USART3);

        if(fPostRcvData != 0)
        {
            fPostRcvData(MY_UART3, data);                   // 将串口读取到的数据上传给使用者
        }
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }

//--处理发送的数据
    if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
    {
        if(fGetSendData != 0)
        {
            if(fGetSendData(MY_UART3, data) == 0)           // 获取要发送的数据
            {
                USART_SendData(USART3, data);
            }
            else
            {
                USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
                fSendEndHandler(MY_UART3);                  // 发送结束处理，为RS485等预留
            }
        }
        USART_ClearITPendingBit(USART3, USART_IT_TXE);
    }

#endif // USART3_EN    
}

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
void fUART4_Init(void)
{
#if (UART4_EN > 0)
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

//--配置串口时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                           | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
                           | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

//--配置中断向量
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART4_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART4_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

//--配置串口引脚
    GPIO_InitStructure.GPIO_Pin  = UART4_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART4_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = UART4_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(UART4_PORT, &GPIO_InitStructure);

//--配置串口参数
    USART_InitStructure.USART_BaudRate            = UART4_BAUD_RATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART4, &USART_InitStructure);

    USART_Cmd(UART4, ENABLE);

    USART_ClearFlag(UART4, USART_FLAG_TC);

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

#endif // UART4_EN
}

/*********************************************************************************************************
** 函数名称 ：UART4_IRQHandler
** 函数功能 ：串口4中断发送接收处理函数
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void UART4_IRQHandler(void)
{
#if (UART4_EN > 0)
    uint8_t data = 0;

//--处理接收的数据
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        data = (uint8_t)USART_ReceiveData(UART4);

        if(fPostRcvData != 0)
        {
            fPostRcvData(MY_UART4, data);                   // 将串口读取到的数据上传给使用者
        }
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
    }

//--处理发送的数据
    if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
    {
        if(fGetSendData != 0)
        {
            if(fGetSendData(MY_UART4, data) == 0)           // 获取要发送的数据
            {
                USART_SendData(UART4, data);
            }
            else
            {
                USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
                fSendEndHandler(MY_UART4);                  // 发送结束处理，为RS485等预留
            }
        }
        USART_ClearITPendingBit(UART4, USART_IT_TXE);
    }

#endif // UART4_EN    
}

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
void fUART5_Init(void)
{
#if (UART5_EN > 0)
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

//--配置串口时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                           | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
                           | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

//--配置中断向量
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART5_PREEMPTION_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART5_SUB_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

//--配置串口引脚
    GPIO_InitStructure.GPIO_Pin  = UART5_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART5_TX_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = UART5_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(UART5_RX_PORT, &GPIO_InitStructure);

//--配置串口参数
    USART_InitStructure.USART_BaudRate            = UART5_BAUD_RATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(UART5, &USART_InitStructure);

    USART_Cmd(UART5, ENABLE);

    USART_ClearFlag(UART5, USART_FLAG_TC);

    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

#endif // UART5_EN
}

/*********************************************************************************************************
** 函数名称 ：UART5_IRQHandler
** 函数功能 ：串口5中断发送接收处理函数
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-09
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void UART5_IRQHandler(void)
{
#if (UART5_EN > 0)
    uint8_t data = 0;

//--处理接收的数据
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        data = (uint8_t)USART_ReceiveData(UART5);

        if(fPostRcvData != 0)
        {
            fPostRcvData(MY_UART5, data);                   // 将串口读取到的数据上传给使用者
        }
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }

//--处理发送的数据
    if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
    {
        if(fGetSendData != 0)
        {
            if(fGetSendData(MY_UART5, data) == 0)           // 获取要发送的数据
            {
                USART_SendData(UART5, data);
            }
            else
            {
                USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
                fSendEndHandler(MY_UART5);                  // 发送结束处理，为RS485等预留
            }
        }
        USART_ClearITPendingBit(UART5, USART_IT_TXE);
    }

#endif // UART5_EN    
}

/*********************************************************************************************************
** 函数名称 ：fUSART_SendArrayData
** 函数功能 ：串口等待式发送数据
** 入口参数 ：nUart: 串口号
**            array: 发送的数组
**			  length: 数组长度
** 出口参数 ：成功:0; 失败:1;
** 作者日期 ：Jason 2020-12-10
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fUSART_SendArrayData(e_UartName nUart,const uint8_t *array,const uint32_t length)
{
    uint32_t i = 0;
    uint32_t overTime = 0;
    USART_TypeDef *UARTx[5] = {USART1, USART2, USART3, UART4, UART5};

    for(i = 0; i < length; i++)
    {
        overTime = 0;

        while(USART_GetFlagStatus(UARTx[nUart],USART_FLAG_TC)!=SET) // 等待发送完毕
        {
            if(overTime++ > 3000)                                   // 防止卡死(9600时，1445刚刚好，预留一倍以上设置为3000)
            {
                return 1;
            }
        }

        USART_SendData(UARTx[nUart], array[i]);
    }
    return 0;
}

/****************************************END OF FILE****************************************************/
