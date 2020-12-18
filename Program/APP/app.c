/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部
**                                  http://www.maiwei.com.cn
**------------------------------------------------------------------------------------------------------
** 文件名称 : app.c
** 内容描述 :
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
#include "includes.h"

#define  APP_TASK_START_PRIO                        2
#define  APP_TASK_START_STK_SIZE                    128
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
static  void  AppTaskStart  (void *p_arg);


/*********************************************************************************************************
** 函数名称 ：fCPU_DisableAllInterrupts
** 函数功能 ：CPU禁止所有中断
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static void  fCPU_DisableAllInterrupts (void)
{
    CPU_IntDis();
}

/*********************************************************************************************************
** 函数名称 ：fRTOS_Start
** 函数功能 ：
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void  fRTOS_Start (void)
{
    OS_ERR  err;

    fCPU_DisableAllInterrupts();

    OSInit(&err);

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);   /* Start multitasking (i.e. give control to uC/OS-III). */
}

/*********************************************************************************************************
** 函数名称 ：fAppTaskCreate
** 函数功能 ：创建任务
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static  void  fAppTaskCreate (void)
{
    
}

/*********************************************************************************************************
** 函数名称 ：fAppObjCreate
** 函数功能 ：创建事件
** 入口参数 ：None
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static  void  fAppObjCreate (void)
{
    
}

/*********************************************************************************************************
** 函数名称 ：AppTaskStart
** 函数功能 ：
** 入口参数 ：p_arg: [输入/出]
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-08
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpuClkFreq;
    CPU_INT32U  cnts;
    OS_ERR      err;
    
//    u16 led0pwmval = 0;
//    u8  dir = 1;
    
//uint8_t array[10] = {0,1,2,3,4,5,6,7,8,9};

static S_GregorianCalendarTimeDef timer = {0};

    (void)p_arg;

    fHAL_Iint();
    
    while(fENC28J60_Init());
//    fHDL_Init();
    CPU_Init();

    cpuClkFreq = fGetCPU_ClkFreq();
    cnts = cpuClkFreq / (CPU_INT32U)OSCfg_TickRate_Hz;
    OS_CPU_SysTickInit(cnts);                                   // 初始化CPU SysTick

    Mem_Init();                                                 // 初始化内存管理模块

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               // 任务运行前初始化CPU使用率统计功能
#endif

    CPU_IntDisMeasMaxCurReset();

    fAppTaskCreate();                                            // 创建应用程序任务

    fAppObjCreate();                                             // 创建应用程序Events
    
//    USART_SendData(USART2, 0x66);
    fRTC_Set(2020,12,11,8,0,0);
    for(;;)                                                     // 任务主体，总是写成一个无限循环。
    {
//        if(dir)
//            led0pwmval++;
//        else 
//            led0pwmval--;
//        if(led0pwmval>720)
//            dir=0;
//        if(led0pwmval==0)
//            dir=1;
//        TIM_SetCompare1(TIM3,led0pwmval);
//        TIM_SetCompare2(TIM3,led0pwmval);
//        fUSART_SendArrayData(MY_UART1,array,10);
//        fUSART_SendArrayData(MY_UART2,array,10);
        fRTC_Get(&timer);
//        timer.sec++;
        OSTimeDlyHMSM(0, 0, 1, 0,OS_OPT_TIME_HMSM_STRICT, &err);  
//        LED1_OFF;
//        LED2_ON;
//        OSTimeDlyHMSM(0, 0, 0, 150,OS_OPT_TIME_HMSM_STRICT, &err);
//        LED2_OFF;
//        LED3_ON;
//        OSTimeDlyHMSM(0, 0, 0, 150,OS_OPT_TIME_HMSM_STRICT, &err);
//        LED3_OFF;
//        LED4_ON;
//        OSTimeDlyHMSM(0, 0, 0, 150,OS_OPT_TIME_HMSM_STRICT, &err);
//        LED4_OFF;
//        LED3_ON;
//        OSTimeDlyHMSM(0, 0, 0, 150,OS_OPT_TIME_HMSM_STRICT, &err);
//        LED3_OFF;
//        LED2_ON;
//        OSTimeDlyHMSM(0, 0, 0, 150,OS_OPT_TIME_HMSM_STRICT, &err);
//        LED2_OFF;
//        LED1_ON;
//        OSTimeDlyHMSM(0, 0, 0, 150,OS_OPT_TIME_HMSM_STRICT, &err);

    }
}

/****************************************END OF FILE****************************************************/
