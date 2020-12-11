/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部
**                                  http://www.maiwei.com.cn
**------------------------------------------------------------------------------------------------------
** 文件名称 : hal_rtc.c
** 内容描述 :
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
#include "hal_rtc.h"
#if (DEF_STM32F10X_RTC_EN > 0)
/*********************************************配置区****************************************************/

/*********************************************变量区****************************************************/

static const uint8_t monTable[12]= {31,28,31,30,31,30,31,31,30,31,30,31}; // 平年的月份日期表

/*********************************************声明区****************************************************/

#endif //DEF_STM32F10X_RTC_EN
/******************************************内部函数区****************************************************/

/*********************************************************************************************************
** 函数名称 ：fIsLeapYear
** 函数功能 ：判断是否为闰年
** 入口参数 ：year: 年份
** 出口参数 ：闰年:1 否则:0
** 作者日期 ：Jason 2020-12-11
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
#if (DEF_STM32F10X_RTC_EN > 0)
static uint8_t fIsLeapYear(uint16_t year)
{
    if(year%4==0) //必须能被4整除
    {
        if(year%100==0)
        {
            if(year%400==0)return 1;//如果以00结尾,还要能被400整除
            else return 0;
        } else return 1;
    } else return 0;
}
#endif //DEF_STM32F10X_RTC_EN
/******************************************接口函数区****************************************************/

/*********************************************************************************************************
** 函数名称 ：fRTC_Init
** 函数功能 ：RTC初始化
** 入口参数 ：None
** 出口参数 ：成功:0 失败:1
** 作者日期 ：Jason 2020-12-11
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fRTC_Init(void)
{
#if (DEF_STM32F10X_RTC_EN > 0)
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // 使能 PWR 和 BKP 外设时钟

    PWR_BackupAccessCmd(ENABLE);                                             // 使能后备寄存器访问

    if(BKP_ReadBackupRegister(BKP_DR1) == 0xA5A5)                            // 从指定的后备寄存器中读出数据,如果没有掉电，则不需要重新配置
    {
        return 0;
    }

    BKP_DeInit();                                                            // 复位备份区域

    RCC_LSEConfig(RCC_LSE_ON);                                               // 设置外部低速晶振(LSE)

    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)                       // 检查指定的 RCC 标志位设置与否,等待低速晶振就绪
    {
    }

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);                                  // 设置 RTC 时钟，选择 LSE 作为 RTC 时钟

    RCC_RTCCLKCmd(ENABLE);                                                   // 使能 RTC 时钟

    RTC_WaitForLastTask();                                                   // 等待最近一次对 RTC 寄存器的写操作完成

    RTC_WaitForSynchro();                                                    // 等待 RTC 寄存器同步

    RTC_WaitForLastTask();                                                   // 等待最近一次对 RTC 寄存器的写操作完成

    RTC_ITConfig(RTC_IT_SEC, ENABLE);                                        // 使能 RTC 秒中断

    RTC_WaitForLastTask();                                                   // 等待最近一次对 RTC 寄存器的写操作完成

    RTC_EnterConfigMode();                                                   // 允许配置

    RTC_SetPrescaler(32767);                                                 // 设置 RTC 预分频的值

    RTC_WaitForLastTask();                                                   // 等待最近一次对 RTC 寄存器的写操作完成

    fRTC_Set(1970,1,1,8,0,0);                                               // 设置时间

    RTC_ExitConfigMode();                                                    // 退出配置模式

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);                                // 向指定的后备寄存器中
    
#endif //DEF_STM32F10X_RTC_EN

    return 0;                                                                // 成功
}

/*********************************************************************************************************
** 函数名称 ：fRTC_Set
** 函数功能 ：设置RTC时间
** 入口参数 ：year : 年(1970~2099)
**			  mon  : 月
**			  day  : 日
**			  hour : 时
**			  min  : 分
**			  sec  : 秒
** 出口参数 ：成功:0 失败:1
** 作者日期 ：Jason 2020-12-11
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fRTC_Set(uint16_t year, uint8_t mon, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
#if (DEF_STM32F10X_RTC_EN > 0)
    uint16_t time;
    uint32_t seccount = 0;

    if((year < 1970) || (year > 2099))
    {
        return 1;
    }

    for(time = 1970; time < year; time++)	              // 把所有年份的秒钟相加
    {
        if(fIsLeapYear(time))
        {
            seccount += 31622400;                         // 闰年的秒钟数
        }
        else
        {
            seccount += 31536000;			              // 平年的秒钟数
        }
    }

    mon -= 1;

    for(time = 0; time < mon; time ++)	                  // 把前面月份的秒钟数相加
    {
        seccount += (uint32_t)monTable[time] * 86400;          // 月份秒钟数相加
        if(fIsLeapYear(year) && time == 1)
        {
            seccount+=86400;                              // 闰年2月份增加一天的秒钟数
        }
    }

    seccount += (uint32_t)(day - 1) * 86400;                  // 把前面日期的秒钟数相加
    seccount += (uint32_t)hour * 3600;	                     // 小时秒钟数
    seccount += (uint32_t)min * 60;	 	                     // 分钟秒钟数
    seccount += sec;						             // 最后的秒钟加上去

    RTC_WaitForLastTask();
    RTC_SetCounter(seccount);                            // 设置时钟
    RTC_WaitForLastTask();
    
#endif //DEF_STM32F10X_RTC_EN
    return 0;
}

/*********************************************************************************************************
** 函数名称 ：RTC_Get_Week
** 函数功能 ：输入公历日期得到星期(只允许1901-2099年)
** 入口参数 ：year : 年(1970~2099)
**			  mon  : 月
**			  day  : 日
** 出口参数 ：星期号
** 作者日期 ：Jason 2020-12-11
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fRTC_GetWeek(uint16_t year,uint8_t month,uint8_t day)
{
#if (DEF_STM32F10X_RTC_EN > 0)
    uint16_t week;
    uint8_t  yearH,yearL;
    const uint8_t tableWeek[12]= {0,3,3,6,1,4,6,2,5,0,3,5};           // 月修正数据表,月份数据表

    yearH = year / 100;
    yearL = year % 100;

    if (yearH > 19)
    {
        yearL += 100;	        // 如果为21世纪,年份数加100
    }

    week = yearL + yearL / 4;  // 所过闰年数只算1900年之后的
    week = week % 7;
    week = week + day + tableWeek[month - 1];

    if (yearL % 4 == 0 && month < 3)
    {
        week--;
    }
    return(week % 7);
#else
    return 0;
#endif //DEF_STM32F10X_RTC_EN
}

/*********************************************************************************************************
** 函数名称 ：fRTC_Get
** 函数功能 ：获取当年公历时间
** 入口参数 ：S_GregorianCalendarTimeDef类型指针,且必须为static。
** 出口参数 ：成功:0 失败:1
** 作者日期 ：Jason 2020-12-11
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fRTC_Get(S_GregorianCalendarTimeDef *timer)
{
#if (DEF_STM32F10X_RTC_EN > 0)
    uint32_t SecTimeCnt        = 0;                              // 总秒数
    uint32_t presentDayCnt     = 0;                              // 本次查询到的天数
    uint16_t presentYearCnt    = 0;                              // 本次查询到的年份
    static uint16_t lastDayCnt = 0;                              // 上次查询到的天数
   
    if(timer == 0)                                               // 空指针
    {
        return 1;
    }
    
    SecTimeCnt = RTC_GetCounter();      
            
    presentDayCnt = SecTimeCnt / 86400;                          // 得到天数(秒钟数对应的)
                                                                 
    if(lastDayCnt != presentDayCnt)                              // 超过一天了
    {                                                            
        lastDayCnt = presentDayCnt;               
        
        presentYearCnt  = 1970;	                                 // 从1970年开始
        
        while(presentDayCnt >= 365)                              
        {                                                        
            if(fIsLeapYear(presentYearCnt))                      // 是闰年
            {                                                    
                if(presentDayCnt >= 366)                         
                {                                                
                    presentDayCnt -= 366;                        // 闰年的天数
                }                                                
                else                                             
                {                                                
                    presentYearCnt++;                            
                    break;                                       
                }                                                
            }                                                    
            else                                                 
            {                                                    
                presentDayCnt -= 365;	                         // 平年
            }       
            presentYearCnt++;       
        } 
        
        timer->year = presentYearCnt;                           // 得到年份
        
        presentYearCnt = 0;       
        
        while(presentDayCnt >= 28)                              // 超过了一个月
        {
            if(fIsLeapYear(timer->year) && (presentYearCnt == 1))// 当年是不是闰年/2月份
            {
                if(presentDayCnt >= 29)
                {
                    presentDayCnt -= 29;                         // 闰年的秒钟数
                }
                else 
                {
                    break;
                }
            }
            else
            {
                if(presentDayCnt >= monTable[presentYearCnt])
                {
                    presentDayCnt -= monTable[presentYearCnt];  //平年
                }
                else 
                {
                    break;
                }
            }
            presentYearCnt++;
        }
        timer->month = presentYearCnt+1;                        // 得到月份
        timer->date  = presentDayCnt+1;                         // 得到日期
    }                           
    presentDayCnt  =  SecTimeCnt%86400;                         // 得到秒钟数
    timer->hour =  presentDayCnt/3600;                          // 小时
    timer->min  = (presentDayCnt%3600)/60;                      // 分钟
    timer->sec  = (presentDayCnt%3600)%60;                      // 秒钟
    timer->week =  fRTC_GetWeek(timer->year,timer->month,timer->date); // 获取星期
    return 0;
//    bit5～bit0：秒， bit11～bit6：分， bit16～bit12：时， bit21～bit17：日，bit25～bit22：月，bit26～bit31:年。
//    Time=((timer.sec<<0) |(timer.min<<6) |(timer.hour<<12) |(timer.w_date<<17) |(timer.w_month<<22) |((timer.w_year-2000)<<26));

#else
    return 0;
#endif //DEF_STM32F10X_RTC_EN
}

/****************************************END OF FILE****************************************************/
