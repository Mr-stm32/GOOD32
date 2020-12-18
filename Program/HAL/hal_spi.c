/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部 
**                                  http://www.maiwei.com.cn   
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_spi.c 
** 内容描述 : stm32f103vet6平台下spi1和spi2初始化。
**		  					
**------------------------------------------------------------------------------------------------------
** 创 建 者 : Jason
** 版    本 : V1.0
** 创建日期 : 2020-12-10 
**------------------------------------------------------------------------------------------------------
** 修 改 者 : None	
** 版    本 : None	
** 修改日期 : None	
** 内容描述 : None	
********************************************************************************************************/
#include "hal_spi.h"

#if (DEF_STM32F10X_SPI_EN > 0)
/*********************************************配置区****************************************************/



/*********************************************变量区****************************************************/



/*********************************************声明区****************************************************/



#endif //DEF_STM32F10X_SPI_EN
/******************************************内部函数区****************************************************/


/******************************************接口函数区****************************************************/
/*********************************************************************************************************
** 函数名称 ：fSPI1_Init
** 函数功能 ：SPI1初始化
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-10
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fSPI1_Init(void)
{
#if (SPI1_EN > 0)
    
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

//--开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                         | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD 
                         | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE); 
    
//--配置IO PA5--C，PA6--Q，PA7--D，
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
//--配置片选脚
    GPIO_InitStructure.GPIO_Pin   = SPI1_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(SPI1_CS_PORT, &GPIO_InitStructure);


//--失能片选脚
    SPI1_CS_DISSELECT;

//--SPI1配置
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;  // 全双工模式
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                  // 主机
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                  // 数据8bit
    SPI_InitStructure.SPI_CPOL              = SPI1_CPOL;                        // 设置时钟极性，同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA              = SPI1_CPHA;                        // 串行同步时钟的第几个跳变沿数据被采样，我们选择第二个跳变沿
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                     // NSS 信号由硬件（NSS 管脚）还是软件控制，我们选择软控
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI1_BRP;                         // SPI速度，36M / 4 = 8M     
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;                 // 设置数据传输顺序是 MSB 位在前还是 LSB 位在前
    SPI_InitStructure.SPI_CRCPolynomial     = 7;                                // 设置 CRC 校验多项式
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);   
    
#endif //SPI1_EN
}


/*********************************************************************************************************
** 函数名称 ：fSPI1_FlashSendByte
** 函数功能 ：发送数据
** 入口参数 ：byte: 发送的数据
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-10
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fSPI1_FlashSendByte(uint8_t byte)	
{
#if (SPI1_EN > 0)
    
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  SPI_I2S_SendData(SPI1, byte);

  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  return SPI_I2S_ReceiveData(SPI1);
#else
    return 0;
#endif //SPI1_EN 
}

/*********************************************************************************************************
** 函数名称 ：fSPI1_FlashReadByte
** 函数功能 ：读取数据
** 入口参数 ：None
** 出口参数 ：读取到的数据
** 作者日期 ：Jason 2020-12-10
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fSPI1_FlashReadByte(void)	   
{
#if (SPI1_EN > 0)
  return (fSPI1_FlashSendByte(0xA5));
#else
    return 0;
#endif //SPI1_EN  
}

/*********************************************************************************************************
** 函数名称 ：fSPI2_Init
** 函数功能 ：SPI2初始化
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-10
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fSPI2_Init(void)
{
#if (SPI2_EN > 0)
    
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

//--开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                         | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD 
                         | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE); 
    
//--配置IO PB13--C，PB14--MISO，PB15--MOSI，
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
//--配置片选脚
    GPIO_InitStructure.GPIO_Pin   = SPI2_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(SPI2_CS_PORT, &GPIO_InitStructure);

//--失能片选脚
    SPI2_CS_DISSELECT;

//--SPI2配置
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;  // 全双工模式
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                  // 主机
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                  // 数据8bit
    SPI_InitStructure.SPI_CPOL              = SPI2_CPOL;                        // 设置时钟极性，同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA              = SPI2_CPHA;                        // 串行同步时钟的第几个跳变沿数据被采样，我们选择第二个跳变沿
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                     // NSS 信号由硬件（NSS 管脚）还是软件控制，我们选择软控
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI2_BRP;                         // SPI速度，36M / 4 = 8M
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;                 // 设置数据传输顺序是 MSB 位在前还是 LSB 位在前
    SPI_InitStructure.SPI_CRCPolynomial     = 7;                                // 设置 CRC 校验多项式
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);   
    
#endif //SPI2_EN
}

/*********************************************************************************************************
** 函数名称 ：fSPI2_FlashSendByte
** 函数功能 ：发送数据
** 入口参数 ：byte: 发送的数据
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-10
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fSPI2_FlashSendByte(uint8_t byte)	
{
#if (SPI2_EN > 0)
    
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  SPI_I2S_SendData(SPI2, byte);

  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  return SPI_I2S_ReceiveData(SPI2);
#else
    return 0;
#endif //SPI2_EN 
}

/*********************************************************************************************************
** 函数名称 ：fSPI2_FlashReadByte
** 函数功能 ：读取数据
** 入口参数 ：None
** 出口参数 ：读取到的数据
** 作者日期 ：Jason 2020-12-10
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fSPI2_FlashReadByte(void)	   
{
#if (SPI2_EN > 0)
  return (fSPI2_FlashSendByte(0xA5));
#else
    return 0;
#endif //SPI2_EN  
}

/****************************************END OF FILE****************************************************/
