/****************************************Copyright (C)**************************************************
**                                  泉州迈韦通信技术有限公司
**                                         研发部 
**                                  http://www.maiwei.com.cn   
**------------------------------------------------------------------------------------------------------
** 文件名称 : hdl_enc28j60.c 
** 内容描述 : 
**		  					
**------------------------------------------------------------------------------------------------------
** 创 建 者 : Jason
** 版    本 : V1.0
** 创建日期 : 2020-12-17 
**------------------------------------------------------------------------------------------------------
** 修 改 者 : None	
** 版    本 : None	
** 修改日期 : None	
** 内容描述 : None	
********************************************************************************************************/
#include "hdl_enc28j60.h"
/*********************************************配置区****************************************************/

//--地址掩码
#define ADDR_MASK        0x1F // 寄存器地址掩码

#define BANK_MASK        0x60 // 存储区域掩码

#define SPRD_MASK        0x80 // MAC和MII寄存器掩码

//--所有Bank共有的寄存器
#define EIE              0x1B // 以太网中断允许寄存器
#define EIR              0x1C // 以太网中断请求 （标志）寄存器
#define ESTAT            0x1D // 以太网状态寄存器
#define ECON2            0x1E // 以太网控制寄存器 2
#define ECON1            0x1F // 以太网控制寄存器 1

//--Bank 0寄存器
#define ERDPTL           (0x00|0x00) // 读指针低字节
#define ERDPTH           (0x01|0x00) // 读指针高字节
#define EWRPTL           (0x02|0x00) // 写指针低字节 
#define EWRPTH           (0x03|0x00) // 写指针高字节 
#define ETXSTL           (0x04|0x00) // 发送起始低字节
#define ETXSTH           (0x05|0x00) // 发送起始高字节
#define ETXNDL           (0x06|0x00) // 发送结束低字节
#define ETXNDH           (0x07|0x00) // 发送结束高字节
#define ERXSTL           (0x08|0x00) // 接收起始低字节
#define ERXSTH           (0x09|0x00) // 接收起始高字节
#define ERXNDL           (0x0A|0x00) // 接收结束低字节
#define ERXNDH           (0x0B|0x00) // 接收结束高字节
                                     // ERXWRPTH:ERXWRPTL 寄存器定义硬件向FIFO中的哪个位置写入其接收到的字节。 
                                     // 指针是只读的，在成功接收到一个数据包后，硬件会自动更新指针。 
                                     // 指针可用于判断FIFO 内剩余空间的大小。
#define ERXRDPTL         (0x0C|0x00) // 接收读指针低字节
#define ERXRDPTH         (0x0D|0x00) // 接收读指针高字节
#define ERXWRPTL         (0x0E|0x00) // 接收写指针低字节
#define ERXWRPTH         (0x0F|0x00) // 接收写指针高字节
#define EDMASTL          (0x10|0x00) // DMA 起始低字节
#define EDMASTH          (0x11|0x00) // DMA 起始高字节
#define EDMANDL          (0x12|0x00) // DMA 结束低字节
#define EDMANDH          (0x13|0x00) // DMA 结束高字节
#define EDMADSTL         (0x14|0x00) // DMA 目标低字节 
#define EDMADSTH         (0x15|0x00) // DMA 目标高字节 
#define EDMACSL          (0x16|0x00) // DMA 校验和低字节 
#define EDMACSH          (0x17|0x00) // DMA 校验和高字节

//--Bank 1寄存器
#define EHT0             (0x00|0x20) // 哈希表字节 0
#define EHT1             (0x01|0x20) // 
#define EHT2             (0x02|0x20) // 
#define EHT3             (0x03|0x20) // 
#define EHT4             (0x04|0x20) // 
#define EHT5             (0x05|0x20) // 
#define EHT6             (0x06|0x20) // 
#define EHT7             (0x07|0x20) // 
#define EPMM0            (0x08|0x20) // 格式匹配屏蔽字节 0
#define EPMM1            (0x09|0x20) // 
#define EPMM2            (0x0A|0x20) // 
#define EPMM3            (0x0B|0x20) // 
#define EPMM4            (0x0C|0x20) // 
#define EPMM5            (0x0D|0x20) // 
#define EPMM6            (0x0E|0x20) // 
#define EPMM7            (0x0F|0x20) // 
#define EPMCSL           (0x10|0x20) // 格式匹配校验和低字节 
#define EPMCSH           (0x11|0x20) // 格式匹配校验和高字节 
#define EPMOL            (0x14|0x20) // 格式匹配偏移地址低字节
#define EPMOH            (0x15|0x20) // 格式匹配偏移地址高字节
#define EWOLIE           (0x16|0x20) // 
#define EWOLIR           (0x17|0x20) // 
#define ERXFCON          (0x18|0x20) // 
#define EPKTCNT          (0x19|0x20) // 

//--Bank 2寄存器
#define MACON1           (0x00|0x40|0x80)
#define MACON2           (0x01|0x40|0x80)
#define MACON3           (0x02|0x40|0x80)
#define MACON4           (0x03|0x40|0x80)
#define MABBIPG          (0x04|0x40|0x80) // 背对背包间间隔 
#define MAIPGL           (0x06|0x40|0x80) // 非背对背包间间隔低字节
#define MAIPGH           (0x07|0x40|0x80) // 非背对背包间间隔高字节
#define MACLCON1         (0x08|0x40|0x80) // 重发最大次数
#define MACLCON2         (0x09|0x40|0x80) // 冲突窗口 
#define MAMXFLL          (0x0A|0x40|0x80) // 最大帧长度低字节
#define MAMXFLH          (0x0B|0x40|0x80) // 最大帧长度高字节
#define MAPHSUP          (0x0D|0x40|0x80) // 
#define MICON            (0x11|0x40|0x80) // 
#define MICMD            (0x12|0x40|0x80) // 
#define MIREGADR         (0x14|0x40|0x80) // MII 寄存器地址 
#define MIWRL            (0x16|0x40|0x80) // MII 写数据低字节 
#define MIWRH            (0x17|0x40|0x80) // MII 写数据高字节 
#define MIRDL            (0x18|0x40|0x80) // MII 读数据低字节
#define MIRDH            (0x19|0x40|0x80) // MII 读数据高字节

//--Bank 3 寄存器
#define MAADR1           (0x00|0x60|0x80) // MAC 地址字节 1 
#define MAADR0           (0x01|0x60|0x80) // MAC 地址字节 0
#define MAADR3           (0x02|0x60|0x80) // 
#define MAADR2           (0x03|0x60|0x80) // 
#define MAADR5           (0x04|0x60|0x80) // 
#define MAADR4           (0x05|0x60|0x80) // 
#define EBSTSD           (0x06|0x60)      // 内置自测试填充种子
#define EBSTCON          (0x07|0x60)      // 
#define EBSTCSL          (0x08|0x60)      // 内置自测试校验和低字节
#define EBSTCSH          (0x09|0x60)      // 内置自测试校验和高字节
#define MISTAT           (0x0A|0x60|0x80) // 
#define EREVID           (0x12|0x60)      // 以太网版本 ID
#define ECOCON           (0x15|0x60)      // 
#define EFLOCON          (0x17|0x60)      // 
#define EPAUSL           (0x18|0x60)      // 暂停定时器值低字节 
#define EPAUSH           (0x19|0x60)      // 暂停定时器值高字节

//--PHY寄存器
#define PHCON1           0x00
#define PHSTAT1          0x01
#define PHHID1           0x02
#define PHHID2           0x03
#define PHCON2           0x10
#define PHSTAT2          0x11
#define PHIE             0x12
#define PHIR             0x13
#define PHLCON           0x14

//--ENC28J60 ERXFCON寄存器定义
#define ERXFCON_UCEN     0x80
#define ERXFCON_ANDOR    0x40
#define ERXFCON_CRCEN    0x20
#define ERXFCON_PMEN     0x10
#define ERXFCON_MPEN     0x08
#define ERXFCON_HTEN     0x04
#define ERXFCON_MCEN     0x02
#define ERXFCON_BCEN     0x01
//--ENC28J60 EIE寄存器位定义
#define EIE_INTIE        0x80
#define EIE_PKTIE        0x40
#define EIE_DMAIE        0x20
#define EIE_LINKIE       0x10
#define EIE_TXIE         0x08
#define EIE_WOLIE        0x04
#define EIE_TXERIE       0x02
#define EIE_RXERIE       0x01
//--ENC28J60 EIR寄存器位定义
#define EIR_PKTIF        0x40
#define EIR_DMAIF        0x20
#define EIR_LINKIF       0x10
#define EIR_TXIF         0x08
#define EIR_WOLIF        0x04
#define EIR_TXERIF       0x02
#define EIR_RXERIF       0x01
//--ENC28J60 ESTAT寄存器位定义
#define ESTAT_INT        0x80
#define ESTAT_LATECOL    0x10
#define ESTAT_RXBUSY     0x04
#define ESTAT_TXABRT     0x02
#define ESTAT_CLKRDY     0x01
//--NC28J60 ECON2寄存器位定义
#define ECON2_AUTOINC    0x80
#define ECON2_PKTDEC     0x40
#define ECON2_PWRSV      0x20
#define ECON2_VRPS       0x08
//--NC28J60 ECON1寄存器位定义
#define ECON1_TXRST      0x80
#define ECON1_RXRST      0x40
#define ECON1_DMAST      0x20
#define ECON1_CSUMEN     0x10
#define ECON1_TXRTS      0x08
#define ECON1_RXEN       0x04
#define ECON1_BSEL1      0x02
#define ECON1_BSEL0      0x01
//--ENC28J60 MACON1寄存器位定义
#define MACON1_LOOPBK    0x10
#define MACON1_TXPAUS    0x08
#define MACON1_RXPAUS    0x04
#define MACON1_PASSALL   0x02
#define MACON1_MARXEN    0x01
//--ENC28J60 MACON2寄存器位定义
#define MACON2_MARST     0x80
#define MACON2_RNDRST    0x40
#define MACON2_MARXRST   0x08
#define MACON2_RFUNRST   0x04
#define MACON2_MATXRST   0x02
#define MACON2_TFUNRST   0x01
//--ENC28J60 MACON3寄存器位定义
#define MACON3_PADCFG2   0x80
#define MACON3_PADCFG1   0x40
#define MACON3_PADCFG0   0x20
#define MACON3_TXCRCEN   0x10
#define MACON3_PHDRLEN   0x08
#define MACON3_HFRMEN    0x04
#define MACON3_FRMLNEN   0x02
#define MACON3_FULDPX    0x01
//--ENC28J60 MICMD寄存器位定义
#define MICMD_MIISCAN    0x02
#define MICMD_MIIRD      0x01
//--ENC28J60 MISTAT寄存器位定义
#define MISTAT_NVALID    0x04
#define MISTAT_SCAN      0x02
#define MISTAT_BUSY      0x01
//--ENC28J60 PHY PHCON1寄存器位定义
#define PHCON1_PRST      0x8000
#define PHCON1_PLOOPBK   0x4000
#define PHCON1_PPWRSV    0x0800
#define PHCON1_PDPXMD    0x0100
//--ENC28J60 PHY PHSTAT1寄存器位定义
#define PHSTAT1_PFDPX    0x1000
#define PHSTAT1_PHDPX    0x0800
#define PHSTAT1_LLSTAT   0x0004
#define PHSTAT1_JBSTAT   0x0002
//--ENC28J60 PHY PHCON2寄存器位定义
#define PHCON2_FRCLINK   0x4000
#define PHCON2_TXDIS     0x2000
#define PHCON2_JABBER    0x0400
#define PHCON2_HDLDIS    0x0100
//--ENC28J60 Packet Control Byte Bit Definitions
#define PKTCTRL_PHUGEEN  0x08
#define PKTCTRL_PPADEN   0x04
#define PKTCTRL_PCRCEN   0x02
#define PKTCTRL_POVERRIDE 0x01

/* 读控制寄存器 */
#define ENC28J60_READ_CTRL_REG      0x00
/* 读缓冲区 */
#define ENC28J60_READ_BUF_MEM       0x3A
/* 写控制寄存器 */
#define ENC28J60_WRITE_CTRL_REG     0x40
/* 写缓冲区 */
#define ENC28J60_WRITE_BUF_MEM      0x7A
/* 位域置位 */
#define ENC28J60_BIT_FIELD_SET      0x80
/* 位域清零 */
#define ENC28J60_BIT_FIELD_CLR      0xA0
/* 系统复位 */
#define ENC28J60_SOFT_RESET         0xFF

#define RXSTART_INIT 	    0x0				//接收buffer起始位置必须为0	
#define RXSTOP_INIT  	    (0x1FFF-1518-1)	//接收buffer终止位置
#define TXSTART_INIT  	    (0x1FFF-1518)	//发送buffer的起始位置
#define TXSTOP_INIT         0x1FFF			//发送buffer的终止位置
#define MAX_FRAMELEN        1518        	//以太网最大帧长度


/*********************************************变量区****************************************************/
struct ENC28J60_DEV_TypeDef{
	u8  bank;	        // ENC28J60当前使用的控制寄存器块
	u32 nextPacketPtr;	// 读指针
	u8  macAddr[6];		// MAC地址
}c_ENC28J60_Dev;



/*********************************************声明区****************************************************/




/******************************************内部函数区****************************************************/
/*********************************************************************************************************
** 函数名称 ：fENC28J60_WriteOp
** 函数功能 ：写操作码
** 入口参数 ：op:       操作码
**			  address:  地址
**			  data:     数据
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fENC28J60_WriteOp(uint8_t op, uint8_t address, uint8_t data)
{
	uint8_t opAddress = 0;
						  	  
	ENC28J60_CS = 0;                                    // 使能ENC28J60 	     
                            
	opAddress = op | (address & ADDR_MASK);         
        
	ENC28J60_SPI_SENDBYTE(opAddress);                   // 通过SPI发送 操作码和寄存器地址
        
	ENC28J60_SPI_SENDBYTE(data);                        // 通过SPI发送寄存器数值 
                    
	ENC28J60_CS = 1;                                    // 禁止ENC28J60
}


/*********************************************************************************************************
** 函数名称 ：fENC28J60_ReadOp
** 函数功能 ：读操作码
** 入口参数 ：op:      操作码
**			  address: 地址
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static uint8_t fENC28J60_ReadOp(uint8_t op, uint8_t address)
{
	uint8_t opAddressOrData = 0;
	
	ENC28J60_CS = 0;                                    // 使能ENC28J60 	

	opAddressOrData = op | (address & ADDR_MASK);

	ENC28J60_SPI_SENDBYTE(opAddressOrData);             // 通过SPI发送 操作码和寄存器地址

	opAddressOrData = ENC28J60_SPI_SENDBYTE(0XFF);      // 通过SPI读取出数据
	
	if(address & 0x80)                                  // 如果是MAC和MII寄存器，第一个读取的字节无效，该信息包含在地址的最高位
	{
        opAddressOrData = ENC28J60_SPI_SENDBYTE(0xFF);  // 再次通过SPI读取数据
	}
	
	ENC28J60_CS = 1;                                    // 禁止ENC28J60
 
	return opAddressOrData;
}

/*********************************************************************************************************
** 函数名称 ：fENC28J60_SetBank
** 函数功能 ：设置ENC28J60的Bank；一共4个，0-3；
** 入口参数 ：address: bank地址
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static void fENC28J60_SetBank(uint8_t address)
{
	if((address & BANK_MASK) != c_ENC28J60_Dev.bank)                                 // 对比bank是否修改
	{
        fENC28J60_WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0)); // 清除ECON1的BSEL1 BSEL0 详见数据手册15页

        fENC28J60_WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);  // 请注意寄存器地址的宏定义，bit6 bit5代码寄存器存储区域位置

        c_ENC28J60_Dev.bank = (address & BANK_MASK);                                 // 重新确定当前寄存器存储区域
	}
}

/*********************************************************************************************************
** 函数名称 ：fENC28J60_Read
** 函数功能 ：读取address地址处寄存器值
** 入口参数 ：address: 
** 出口参数 ：address地址处寄存器值
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static uint8_t fENC28J60_Read(uint8_t address)
{
	fENC28J60_SetBank(address);                               // 设定寄存器地址区域 
    
	return fENC28J60_ReadOp(ENC28J60_READ_CTRL_REG, address); //读取寄存器值 发送读寄存器命令和地址
}

/*********************************************************************************************************
** 函数名称 ：enc28j60Write
** 函数功能 ：在address地址寄存器写入data值
** 入口参数 ：address: 寄存器地址
**			  data:    需要写入的值
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static void fENC28J60_Write(uint8_t address, uint8_t data)
{
	fENC28J60_SetBank(address);                                 // 设定寄存器地址区域

	fENC28J60_WriteOp(ENC28J60_WRITE_CTRL_REG, address, data);  // 写寄存器值 发送写寄存器命令和地址 
}

/*********************************************************************************************************
** 函数名称 ：fENC28J60_PhyWrite
** 函数功能 ：
** 入口参数 ：address: 地址
**			  data:    数据
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static void fENC28J60_PhyWrite(uint8_t address, uint16_t data)
{
    uint32_t reTry = 0;    
    
	fENC28J60_Write(MIREGADR, address);                         // 向MIREGADR写入地址 详见数据手册19页
                
	fENC28J60_Write(MIWRL, data);                               // 写入低8位数据
                
	fENC28J60_Write(MIWRH, data>>8);                            // 写入高8位数据

	while((fENC28J60_Read(MISTAT) & MISTAT_BUSY) && (reTry++<250))// 等待PHY寄存器写入完成
    {
		ENC28J60_DELAY_MS(1);
    }
}

/*********************************************************************************************************
** 函数名称 ：fENC28J60_PhyRead
** 函数功能 ：读取ENC28J60的PHY寄存器
** 入口参数 ：addr: 要读取的寄存器地址
** 出口参数 ：读取到的PHY的值
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint16_t fENC28J60_PhyRead(u8 addr)
{
	u8 temp = 0;
	u16 phyvalue,reTry = 0;
    
	temp = fENC28J60_Read(MICMD);
    
	fENC28J60_Write(MIREGADR,addr);
    
	fENC28J60_Write(MICMD, temp|MICMD_MIIRD);		// 开始读PHY寄存器
                                                    // 等待读PHY完成
	while((fENC28J60_Read(MISTAT) & MISTAT_BUSY) && reTry < 0XFFFF) reTry++;

	fENC28J60_Write(MICMD,temp & (~MICMD_MIIRD));	// 读PHY完成
                                                       
	phyvalue = fENC28J60_Read(MIRDL);			    // 读取低8位
                                                       
	phyvalue |= (fENC28J60_Read(MIRDH) << 8);	    // 读取高8位

	return phyvalue;
}

/*********************************************************************************************************
** 函数名称 ：fENC28J60_ReadBuffer
** 函数功能 ：从ENC28J60读取数据
** 入口参数 ：length: 长度
**			  data:   数据
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static void fENC28J60_ReadBuffer(uint32_t length, uint8_t* data)
{
	ENC28J60_CS = 0; 
	ENC28J60_SPI_SENDBYTE(ENC28J60_READ_BUF_MEM);
	while(length--)
	{
		*data=(u8)ENC28J60_SPI_SENDBYTE(0xFF);
		data++; 
	}
	ENC28J60_CS = 1;
}

/*********************************************************************************************************
** 函数名称 ：fENC28J60_WriteBuffer
** 函数功能 ：向ENC28J60写入数据
** 入口参数 ：length: 长度
**			  data:   数据
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static void fENC28J60_WriteBuffer(uint32_t length, uint8_t* data)
{
	ENC28J60_CS = 0;
	ENC28J60_SPI_SENDBYTE(ENC28J60_WRITE_BUF_MEM);
	while(length)
	{
		length--;
		ENC28J60_SPI_SENDBYTE(*data);
		data++;
	}
	ENC28J60_CS = 1;
}

/******************************************接口函数区****************************************************/
/*********************************************************************************************************
** 函数名称 ：fENC28J60_Reset
** 函数功能 ：ENC28J60硬件复位
** 入口参数 ：None
** 出口参数 ：成功:0 失败:1
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static uint8_t fENC28J60_Reset(void)
{
    uint32_t reTry = 0;
    
	ENC28J60_RST = 0;			//复位ENC28J60
	ENC28J60_DELAY_MS(10);	 
	ENC28J60_RST = 1;			//复位结束				    
	ENC28J60_DELAY_MS(10);	 
    
	while(!(fENC28J60_Read(ESTAT)&ESTAT_CLKRDY)&&reTry<2500)	//等待时钟稳定
	{
		reTry++;
		ENC28J60_DELAY_MS(1);
	}	
	if(reTry>=250)return 1;      //ENC28J60初始化失败
    return 0;
}

/*********************************************************************************************************
** 函数名称 ：fENC28J60_GetEREVID
** 函数功能 ：在EREVID 内也存储了版本信息。 EREVID 是一个只读控制寄存器，包含一个5 位标识符，
**            用来标识器件特定硅片的版本号
** 入口参数 ：None
** 出口参数 ：ENC28J60版本信息
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
static uint8_t fENC28J60_GetEREVID(void)
{
	return fENC28J60_Read(EREVID)&0X1F;
}

/*********************************************************************************************************
** 函数名称 ：fENC28J60_Init
** 函数功能 ：
** 入口参数 ：None
** 出口参数 ：成功:0 失败:1
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint8_t fENC28J60_Init(void)
{
    uint8_t version = 0;
    uint32_t temp = 0;
	temp = *(vu32*)(0x1FFFF7E8);	                    // 获取STM32的唯一ID的前24位作为MAC地址后三字节
    c_ENC28J60_Dev.macAddr[0] = 2;
    c_ENC28J60_Dev.macAddr[0] = 2;
    c_ENC28J60_Dev.macAddr[0] = 2;
	c_ENC28J60_Dev.macAddr[3] = (temp >> 16) & 0XFF;	// 低三字节用STM32的唯一ID
	c_ENC28J60_Dev.macAddr[4] = (temp >>  8) & 0XFFF;
	c_ENC28J60_Dev.macAddr[5] = (temp&0XFF);
     
    ENC28J60_RET_INIT();
    ENC28J60_SPI_INIT();
    
    if(fENC28J60_Reset() == 1) return 1;                // 复位失败
    
	version=fENC28J60_GetEREVID();			            // 获取ENC28J60的版本号
  
	c_ENC28J60_Dev.nextPacketPtr = RXSTART_INIT;      
	/*接收缓冲器由一个硬件管理的循环FIFO 缓冲器构成。寄存器对ERXSTH:ERXSTL 和ERXNDH:ERXNDL 作为指针，定义缓冲器的容量和其在存储器中的位置。
    ERXST和ERXND指向的字节均包含在FIFO缓冲器内。当从以太网接口接收数据字节时，这些字节被顺序写入接收缓冲器。 
    但是当写入由ERXND 指向的存储单元后，硬件会自动将接收的下一字节写入由ERXST 指向的存储单元。 因此接收硬件将不会写入FIFO 以外的单元。*/
	//--设置接收起始字节	
	fENC28J60_Write(ERXSTL,RXSTART_INIT&0XFF);	//设置接收缓冲区起始地址低8位
	fENC28J60_Write(ERXSTH,RXSTART_INIT>>8);		//设置接收缓冲区起始地址高8位
	//--设置接收接收字节
	fENC28J60_Write(ERXNDL,RXSTOP_INIT&0XFF);	
	fENC28J60_Write(ERXNDH,RXSTOP_INIT>>8);
	//--设置发送起始字节
	fENC28J60_Write(ETXSTL,TXSTART_INIT&0XFF);
	fENC28J60_Write(ETXSTH,TXSTART_INIT>>8);
	//--设置发送结束字节
	fENC28J60_Write(ETXNDL,TXSTOP_INIT&0XFF);
	fENC28J60_Write(ETXNDH,TXSTOP_INIT>>8);
	//ERXWRPTH:ERXWRPTL 寄存器定义硬件向FIFO 中
	//的哪个位置写入其接收到的字节。 指针是只读的，在成
	//功接收到一个数据包后，硬件会自动更新指针。 指针可
	//用于判断FIFO 内剩余空间的大小  8K-1500。 
	//设置接收读指针字节
	fENC28J60_Write(ERXRDPTL,RXSTART_INIT&0XFF);
	fENC28J60_Write(ERXRDPTH,RXSTART_INIT>>8);
	//接收过滤器
	//UCEN：单播过滤器使能位
	//当ANDOR = 1 时：
	//1 = 目标地址与本地MAC 地址不匹配的数据包将被丢弃
	//0 = 禁止过滤器
	//当ANDOR = 0 时：
	//1 = 目标地址与本地MAC 地址匹配的数据包会被接受
	//0 = 禁止过滤器
	//CRCEN：后过滤器CRC 校验使能位
	//1 = 所有CRC 无效的数据包都将被丢弃
	//0 = 不考虑CRC 是否有效
	//PMEN：格式匹配过滤器使能位
	//当ANDOR = 1 时：
	//1 = 数据包必须符合格式匹配条件，否则将被丢弃
	//0 = 禁止过滤器
	//当ANDOR = 0 时：
	//1 = 符合格式匹配条件的数据包将被接受
	//0 = 禁止过滤器
	//fENC28J60_Write(ERXFCON,ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN);
	fENC28J60_Write(ERXFCON,0);
	fENC28J60_Write(EPMM0,0X3F);
	fENC28J60_Write(EPMM1,0X30);
	fENC28J60_Write(EPMCSL,0Xf9);
	fENC28J60_Write(EPMCSH,0Xf7);
	//bit 0 MARXEN：MAC 接收使能位
	//1 = 允许MAC 接收数据包
	//0 = 禁止数据包接收
	//bit 3 TXPAUS：暂停控制帧发送使能位
	//1 = 允许MAC 发送暂停控制帧（用于全双工模式下的流量控制）
	//0 = 禁止暂停帧发送
	//bit 2 RXPAUS：暂停控制帧接收使能位
	//1 = 当接收到暂停控制帧时，禁止发送（正常操作）
	//0 = 忽略接收到的暂停控制帧
	fENC28J60_Write(MACON1,MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
	//将MACON2 中的MARST 位清零，使MAC 退出复位状态。
	fENC28J60_Write(MACON2,0x00);
		//bit 7-5 PADCFG2:PACDFG0：自动填充和CRC 配置位
	//111 = 用0 填充所有短帧至64 字节长，并追加一个有效的CRC
	//110 = 不自动填充短帧
	//101 = MAC 自动检测具有8100h 类型字段的VLAN 协议帧，并自动填充到64 字节长。如果不
	//是VLAN 帧，则填充至60 字节长。填充后还要追加一个有效的CRC
	//100 = 不自动填充短帧
	//011 = 用0 填充所有短帧至64 字节长，并追加一个有效的CRC
	//010 = 不自动填充短帧
	//001 = 用0 填充所有短帧至60 字节长，并追加一个有效的CRC
	//000 = 不自动填充短帧
	//bit 4 TXCRCEN：发送CRC 使能位
	//1 = 不管PADCFG如何，MAC都会在发送帧的末尾追加一个有效的CRC。 如果PADCFG规定要
	//追加有效的CRC，则必须将TXCRCEN 置1。
	//0 = MAC不会追加CRC。 检查最后4 个字节，如果不是有效的CRC 则报告给发送状态向量。
	//bit 0 FULDPX：MAC 全双工使能位
	//1 = MAC工作在全双工模式下。 PHCON1.PDPXMD 位必须置1。
	//0 = MAC工作在半双工模式下。 PHCON1.PDPXMD 位必须清零。
	fENC28J60_Write(MACON3,MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX);
	// 最大帧长度 1518
	fENC28J60_Write(MAMXFLL,MAX_FRAMELEN&0XFF);
	fENC28J60_Write(MAMXFLH,MAX_FRAMELEN>>8);
	//配置背对背包间间隔寄存器MABBIPG。当使用
	//全双工模式时，大多数应用使用15h 编程该寄存
	//器，而使用半双工模式时则使用12h 进行编程。
	fENC28J60_Write(MABBIPG,0x15);
	//配置非背对背包间间隔寄存器的低字节
	//MAIPGL。 大多数应用使用12h 编程该寄存器。
	//如果使用半双工模式，应编程非背对背包间间隔
	//寄存器的高字节MAIPGH。 大多数应用使用0Ch
	//编程该寄存器。
	fENC28J60_Write(MAIPGL,0x12);
	fENC28J60_Write(MAIPGH,0x0C);
	//设置MAC地址
	fENC28J60_Write(MAADR5,c_ENC28J60_Dev.macAddr[0]);
	fENC28J60_Write(MAADR4,c_ENC28J60_Dev.macAddr[1]);
	fENC28J60_Write(MAADR3,c_ENC28J60_Dev.macAddr[2]);
	fENC28J60_Write(MAADR2,c_ENC28J60_Dev.macAddr[3]);
	fENC28J60_Write(MAADR1,c_ENC28J60_Dev.macAddr[4]);
	fENC28J60_Write(MAADR0,c_ENC28J60_Dev.macAddr[5]);
	//配置PHY为全双工  LEDB为拉电流
	fENC28J60_PhyWrite(PHCON1,PHCON1_PDPXMD);	
	//HDLDIS：PHY 半双工环回禁止位
	//当PHCON1.PDPXMD = 1 或PHCON1.PLOOPBK = 1 时：
	//此位可被忽略。
	//当PHCON1.PDPXMD = 0 且PHCON1.PLOOPBK = 0 时：
	//1 = 要发送的数据仅通过双绞线接口发出
	//0 = 要发送的数据会环回到MAC 并通过双绞线接口发出
	fENC28J60_PhyWrite(PHCON2,PHCON2_HDLDIS);
	//ECON1 寄存器
	//寄存器3-1 所示为ECON1 寄存器，它用于控制
	//ENC28J60 的主要功能。 ECON1 中包含接收使能、发
	//送请求、DMA 控制和存储区选择位。	   
	fENC28J60_SetBank(ECON1);
	//EIE： 以太网中断允许寄存器
	//bit 7 INTIE： 全局INT 中断允许位
	//1 = 允许中断事件驱动INT 引脚
	//0 = 禁止所有INT 引脚的活动（引脚始终被驱动为高电平）
	//bit 6 PKTIE： 接收数据包待处理中断允许位
	//1 = 允许接收数据包待处理中断
	//0 = 禁止接收数据包待处理中断
	fENC28J60_WriteOp(ENC28J60_BIT_FIELD_SET,EIE,EIE_INTIE|EIE_PKTIE|EIE_TXIE|EIE_TXERIE|EIE_RXERIE);
	// enable packet reception
	//bit 2 RXEN：接收使能位
	//1 = 通过当前过滤器的数据包将被写入接收缓冲器
	//0 = 忽略所有接收的数据包
	fENC28J60_WriteOp(ENC28J60_BIT_FIELD_SET,ECON1,ECON1_RXEN);
	return 0;
}    

/*********************************************************************************************************
** 函数名称 ：fENC28J60_GetDuplex
** 函数功能 ：获取ENC28J60双工状态
** 入口参数 ：None
** 出口参数 ：1，全双工；0，半双工
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint16_t fENC28J60_GetDuplex(void)
{
	u16 temp;
    
	temp = fENC28J60_PhyRead(PHCON1) & 0X0100;
	return (temp >> 8);
}

/*********************************************************************************************************
** 函数名称 ：fENC28J60_PacketSend
** 函数功能 ：
** 入口参数 ：len:  
**			  packet:  
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
void fENC28J60_PacketSend(unsigned int len, unsigned char* packet)
{
	while((fENC28J60_Read(ECON1) & ECON1_TXRTS)!= 0);               // 查询发送逻辑复位位

	fENC28J60_Write(EWRPTL, TXSTART_INIT & 0xFF);                   // 设置发送缓冲区起始地址
	fENC28J60_Write(EWRPTH, TXSTART_INIT >> 8);     

	fENC28J60_Write(ETXNDL, (TXSTART_INIT + len) & 0xFF);           // 设置发送缓冲区结束地址 该值对应发送数据包长度
	fENC28J60_Write(ETXNDH, (TXSTART_INIT + len) >>8);      

	fENC28J60_WriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);             // 发送之前发送控制包格式字

	fENC28J60_WriteBuffer(len, packet);                             // 通过ENC28J60发送数据包

	fENC28J60_WriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);  // 开始发送

	if( (fENC28J60_Read(EIR) & EIR_TXERIF) )                        // 复位发送逻辑的问题
	{
		fENC28J60_SetBank(ECON1);
        fENC28J60_WriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
    }
}


/*********************************************************************************************************
** 函数名称 ：fENC28J60_PacketReceive
** 函数功能 ：
** 入口参数 ：maxlen: 
**			  packet: 
** 出口参数 ：None
** 作者日期 ：Jason 2020-12-17
**--------------------------------------------------------------------------------------------------------
** 修改作者 ：None
** 修改日期 ：None
** 修改内容 ：None
***********************************************************************************************************/
uint32_t fENC28J60_PacketReceive(uint32_t maxlen, uint8_t* packet)
{
	uint32_t rxstat = 0;
	uint32_t length = 0;

	if(fENC28J60_Read(EPKTCNT) == 0)                                                // 是否收到以太网数据包
	{
		return 0;
    }

	fENC28J60_Write(ERDPTL, (c_ENC28J60_Dev.nextPacketPtr));                        // 设置接收缓冲器读指针 
	fENC28J60_Write(ERDPTH, (c_ENC28J60_Dev.nextPacketPtr)>>8);
 
//--接收数据包结构示例 数据手册43页
	c_ENC28J60_Dev.nextPacketPtr  = fENC28J60_ReadOp(ENC28J60_READ_BUF_MEM, 0);     // 读下一个包的指针
	c_ENC28J60_Dev.nextPacketPtr |= fENC28J60_ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
 
	length  = fENC28J60_ReadOp(ENC28J60_READ_BUF_MEM, 0);                           // 读包的长度
	length |= fENC28J60_ReadOp(ENC28J60_READ_BUF_MEM, 0)<<8;
 
    length-= 4; 	                                                                // 去除CRC校验部分

	rxstat  = fENC28J60_ReadOp(ENC28J60_READ_BUF_MEM, 0);                           // 读取接收状态
	rxstat |= fENC28J60_ReadOp(ENC28J60_READ_BUF_MEM, 0) << 8;

    length = (length > maxlen - 1)?(maxlen - 1):length;                             // 限制检索的长度
  /* 检查CRC和符号错误 */
  /* ERXFCON.CRCEN是默认设置。通常我们不需要检查 */
  if ((rxstat & 0x80)==0)
	{
	   length = 0;//无效的
	}
	else
	{
    /* 从接收缓冲器中复制数据包 */
    fENC28J60_ReadBuffer(length, packet);
  }
 
  /* 移动接收缓冲区 读指针*/
	fENC28J60_Write(ERXRDPTL, (c_ENC28J60_Dev.nextPacketPtr));
	fENC28J60_Write(ERXRDPTH, (c_ENC28J60_Dev.nextPacketPtr)>>8);
 
	/* 数据包递减 */
	fENC28J60_WriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
 
  /* 返回长度 */
	return length;
}

/****************************************END OF FILE****************************************************/
