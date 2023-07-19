#include "w25qxx.h"
#include <stdio.h>
#include "delay.h"

hw_uint16_t W25QXX_TYPE = W25Q64;	// 默认是W25Q64
SPI_HandleTypeDef *dev_w25qxx = &hspi1;
static const uint32_t time_Out = 0XFFFF;

// 4Kbytes为一个Sector
// 16个扇区为1个Block
// W25Q64
// 容量为8M字节,共有128个Block,4096个Sector 

void W25QXX_Init(void)
{
	W25QXX_CS_NENA();
	W25QXX_TYPE = W25QXX_ReadID();  // 读取FLASH ID.
}

static void W25QXX_ReadOneByte(hw_uint8_t *data)
{
	HAL_SPI_Receive(dev_w25qxx, data, 1, time_Out);
}

static void W25QXX_WriteOneByte(const hw_uint8_t *data)
{
	HAL_SPI_Transmit(dev_w25qxx, (hw_uint8_t*)data, 1, time_Out);
}

// 读取W25QXX的状态寄存器
// BIT7  6   5   4   3   2   1   0
// SPR   RV  TB BP2 BP1 BP0 WEL BUSY
// SPR:默认0,状态寄存器保护位,配合WP使用
// TB,BP2,BP1,BP0:FLASH区域写保护设置
// WEL:写使能锁定
// BUSY:忙标记位(1,忙;0,空闲)
// 默认:0x00
hw_uint8_t W25QXX_ReadSR(void)   
{
	hw_uint8_t byte = W25X_ReadStatusReg;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // 发送读取状态寄存器命令
	W25QXX_ReadOneByte(&byte);  // 读取一个字节
	W25QXX_CS_NENA();
	return byte;
}

// 写W25QXX状态寄存器
// 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void W25QXX_Write_SR(hw_uint8_t sr)
{
	hw_uint8_t byte = W25X_ReadStatusReg;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // 发送读取状态寄存器命令
	W25QXX_WriteOneByte(&sr);    // 写入一个字节
	W25QXX_CS_NENA();
}

// W25QXX写使能
// 将WEL置位
void W25QXX_Write_Enable(void)
{
	hw_uint8_t byte = W25X_WriteEnable;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // 发送写使能
	W25QXX_CS_NENA();
}

// W25QXX写禁止	
// 将WEL清零  
void W25QXX_Write_Disable(void)   
{
	hw_uint8_t byte = W25X_WriteDisable;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // 发送写禁止指令
	W25QXX_CS_NENA();
}

// 读取芯片ID
// 返回值如下:				   
// 0XEF13,表示芯片型号为W25Q80  
// 0XEF14,表示芯片型号为W25Q16    
// 0XEF15,表示芯片型号为W25Q32  
// 0XEF16,表示芯片型号为W25Q64 
// 0XEF17,表示芯片型号为W25Q128 	  
hw_uint16_t W25QXX_ReadID(void)
{
	hw_uint16_t Temp = 0;
	hw_uint8_t byte = W25X_ManufactDeviceID;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // 发送读取ID命令
	byte = 0X00;
	W25QXX_WriteOneByte(&byte);
	W25QXX_WriteOneByte(&byte);
	W25QXX_WriteOneByte(&byte);
	W25QXX_ReadOneByte(&byte);
	Temp |= byte<<8;
	W25QXX_ReadOneByte(&byte);
	Temp |= byte;
	W25QXX_CS_NENA();
	return Temp;
}

// 读取SPI FLASH
// 在指定地址开始读取指定长度的数据
// pBuffer:数据存储区
// ReadAddr:开始读取的地址(24bit)
// NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(hw_uint8_t* pBuffer,hw_uint32_t ReadAddr,hw_uint16_t NumByteToRead)
{
 	hw_uint16_t i;
	hw_uint8_t byte = W25X_ReadData;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);							// 发送读取命令
	byte = (hw_uint8_t)((ReadAddr)>>16);				// 发送24bit地址
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)((ReadAddr)>>8);
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)ReadAddr;
	W25QXX_WriteOneByte(&byte);
    for(i = 0; i < NumByteToRead; i++)
	{
		W25QXX_ReadOneByte(&pBuffer[i]);  // 循环读数
    }
	W25QXX_CS_NENA();
}

// SPI在一页(0~65535)内写入少于256个字节的数据
// 在指定地址开始写入最大256字节的数据
// pBuffer:数据存储区
// WriteAddr:开始写入的地址(24bit)
// NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25QXX_Write_Page(hw_uint8_t* pBuffer,hw_uint32_t WriteAddr,hw_uint16_t NumByteToWrite)
{
 	hw_uint16_t i;
    W25QXX_Write_Enable();                  	// SET WEL
	W25QXX_CS_ENA();
	hw_uint8_t byte = W25X_PageProgram;
	W25QXX_WriteOneByte(&byte);      	// 发送写页命令
	byte = (hw_uint8_t)((WriteAddr)>>16);				// 发送24bit地址
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)((WriteAddr)>>8);
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)WriteAddr;
	W25QXX_WriteOneByte(&byte);
    for(i = 0; i < NumByteToWrite; i++)
	{
		W25QXX_WriteOneByte(&pBuffer[i]);  // 循环写数
	}
	W25QXX_CS_NENA();
	W25QXX_Wait_Busy();					   		// 等待写入结束
}

// 无检验写SPI FLASH 
// 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
// 具有自动换页功能 
// 在指定地址开始写入指定长度的数据,但是要确保地址不越界!
// pBuffer:数据存储区
// WriteAddr:开始写入的地址(24bit)
// NumByteToWrite:要写入的字节数(最大65535)
// CHECK OK
void W25QXX_Write_NoCheck(hw_uint8_t* pBuffer, hw_uint32_t WriteAddr, hw_uint16_t NumByteToWrite)
{
	hw_uint16_t pageremain;
	pageremain = 256 - WriteAddr % 256;  // 单页剩余的字节数
	if(NumByteToWrite <= pageremain)  // 不大于256个字节
		pageremain = NumByteToWrite;
	while(1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if(NumByteToWrite == pageremain) break;  // 写入结束了
	 	else  // NumByteToWrite > pageremain
		{
			pBuffer += pageremain;		// 忽略已写入的数据
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain;			  // 减去已经写入了的字节数
			if(NumByteToWrite>256)  // 一次可以写入256个字节
				pageremain = 256;
			else  // 不够256个字节了
				pageremain = NumByteToWrite;
		}
	};
}

// 写SPI FLASH  
// 在指定地址开始写入指定长度的数据
// 该函数带擦除操作!
// pBuffer:数据存储区
// WriteAddr:开始写入的地址(24bit)						
// NumByteToWrite:要写入的字节数(最大65535)   
hw_uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(hw_uint8_t* pBuffer, hw_uint32_t WriteAddr, hw_uint16_t NumByteToWrite)   
{ 
	hw_uint32_t secpos;
	hw_uint16_t secoff;
	hw_uint16_t secremain;	   
 	hw_uint16_t i;    
	hw_uint8_t * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		   	NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	};
}

// 擦除整个芯片		  
// 等待时间超长...
void W25QXX_Erase_Chip(void)   
{
    W25QXX_Write_Enable();                 	 	// SET WEL
    W25QXX_Wait_Busy();
	hw_uint8_t byte = W25X_ChipErase;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);					// 发送片擦除命令
	W25QXX_CS_NENA();
	W25QXX_Wait_Busy();   				   		// 等待芯片擦除结束
}

// 擦除一个扇区
// Dst_Addr:扇区地址 根据实际容量设置
// 擦除一个山区的最少时间:150ms
void W25QXX_Erase_Sector(hw_uint32_t Dst_Addr)   
{
	//监视falsh擦除情况,测试用
 	printf("fe:%x\r\n", Dst_Addr);
 	Dst_Addr *= 4096;
    W25QXX_Write_Enable();                  	// SET WEL
    W25QXX_Wait_Busy();
	hw_uint8_t byte = W25X_SectorErase;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);					// 发送扇区擦除指令
	byte = (hw_uint8_t)((Dst_Addr)>>16);		// 发送24bit地址
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)((Dst_Addr)>>8);
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)Dst_Addr;
	W25QXX_WriteOneByte(&byte);
	W25QXX_CS_NENA();
    W25QXX_Wait_Busy();   				   		// 等待擦除完成
}

// 等待空闲
void W25QXX_Wait_Busy(void)
{
	while((W25QXX_ReadSR()&0x01) == 0x01) HAL_Delay(5);  		// 等待BUSY位清空
}

// 进入掉电模式
void W25QXX_PowerDown(void)
{
	hw_uint8_t byte = W25X_PowerDown;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);					//发送掉电命令
	W25QXX_CS_NENA();
    delay_us(3);								//等待TPD
}

// 唤醒
void W25QXX_WAKEUP(void)
{
	hw_uint8_t byte = W25X_ReleasePowerDown;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);					//  send W25X_PowerDown command 0xAB
	W25QXX_CS_NENA();
    delay_us(3);                            	//等待TRES1
}


























