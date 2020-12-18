/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部 
**                                  http://www.maiwei.com.cn   
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_iwdg.c 
** 内容描述 : stm32f103vet6平台下iwdg初始化。
**		  					
**------------------------------------------------------------------------------------------------------
** 创 建 者 : Jason
** 版    本 : V1.0
** 创建日期 : 2020-12-11 
**------------------------------------------------------------------------------------------------------
** 修 改 者 : None	
** 版    本 : None	
** 修改日期 : None	
** 内容描述 : None	
********************************************************************************************************/
#include "hal_iwdg.h"

/*********************************************配置区****************************************************/

/*********************************************变量区****************************************************/


/*********************************************声明区****************************************************/


/******************************************内部函数区****************************************************/

/********************************************接口区******************************************************/

/*********************************************************************************************************
** 函数名称 ：fIWDG_Init
** 函数功能 ：独立看门狗初始化
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-11
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fIWDG_Init(void)
{
#if (DEF_STM32F10X_IWDG_EN > 0)
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
    IWDG_SetPrescaler((PRESXALER > 6)? 0 : PRESXALER);   
    IWDG_SetReload((RELOAD > 0x0FFF)? 0 : RELOAD);     
    IWDG_ReloadCounter();    
    IWDG_Enable();       
#endif // DEF_STM32F10X_IWDG_EN    
}

/****************************************END OF FILE****************************************************/
