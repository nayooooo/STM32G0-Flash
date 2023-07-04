#include "24cxx.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//24CXX驱动 代码(适合24C01~24C16,24C32~256未经过测试!有待验证!)		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

I2C_HandleTypeDef * dev_24cxx = &hi2c1;
static const uint32_t time_Out = 0XFFFF;
const uint8_t verify_Check_Byte = 0X55;

//初始化IIC接口
void AT24CXX_Init(void)
{
	;
}

void AT24CXX_ReadOneByte(uint16_t ReadAddr,uint8_t *DataToRead)
{
	HAL_I2C_Mem_Read(dev_24cxx, EE_READ,
						ReadAddr, I2C_MEMADD_SIZE_8BIT,
						DataToRead, 1,
						time_Out);
}

void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t *DataToWrite)
{
    HAL_I2C_Mem_Write(dev_24cxx, EE_WRITE,
						WriteAddr, I2C_MEMADD_SIZE_8BIT,
						DataToWrite, 1,
						time_Out);
}

void AT24CXX_ReadPage(uint16_t page,uint8_t *DataToReadBuff)
{
	HAL_I2C_Mem_Read(dev_24cxx, EE_READ,
						page*EE_PAGE_SIZE, I2C_MEMADD_SIZE_8BIT,
						DataToReadBuff, EE_PAGE_SIZE,
						time_Out);
}

void AT24CXX_WritePage(uint16_t page,uint8_t *DataToWriteBuff)
{
    HAL_I2C_Mem_Write(dev_24cxx, EE_WRITE,
						page*EE_PAGE_SIZE, I2C_MEMADD_SIZE_8BIT,
						DataToWriteBuff, EE_PAGE_SIZE,
						time_Out);
}

void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	if (NumToRead < EE_PAGE_SIZE) goto AT24CXX_READ_BY_BYTE;  // 待读出数据数目少于一页数据数目
	if (NumToRead - ReadAddr % EE_PAGE_SIZE < EE_PAGE_SIZE) goto AT24CXX_READ_BY_BYTE;
	else goto AT24CXX_READ_BY_PAGE;
	if (0) {
	AT24CXX_READ_BY_BYTE:
		while(NumToRead--)
		{
			AT24CXX_ReadOneByte(ReadAddr,pBuffer);
			ReadAddr++;
			pBuffer++;
		}
	}
	if (0) {
	AT24CXX_READ_BY_PAGE:
		if (ReadAddr % EE_PAGE_SIZE) {
			while (ReadAddr % EE_PAGE_SIZE) {
				AT24CXX_ReadOneByte(ReadAddr,pBuffer);
				ReadAddr++;
				pBuffer++;
				NumToRead--;
			}
		}
		while (NumToRead >= EE_PAGE_SIZE) {
			uint16_t page = ReadAddr / EE_PAGE_SIZE;
			AT24CXX_ReadPage(page,pBuffer);
			ReadAddr += EE_PAGE_SIZE;
			pBuffer += EE_PAGE_SIZE;
			NumToRead -= EE_PAGE_SIZE;
		}
		goto AT24CXX_READ_BY_BYTE;
	}
}

void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	if (NumToWrite < EE_PAGE_SIZE) goto AT24CXX_WRITE_BY_BYTE;  // 待写入数据数目少于一页数据数目
	if (NumToWrite - WriteAddr % EE_PAGE_SIZE < EE_PAGE_SIZE) goto AT24CXX_WRITE_BY_BYTE;
	else goto AT24CXX_WRITE_BY_PAGE;
	if (0) {
	AT24CXX_WRITE_BY_BYTE:
		while(NumToWrite--)
		{
			AT24CXX_WriteOneByte(WriteAddr,pBuffer);
			HAL_Delay(5);
			WriteAddr++;
			pBuffer++;
		}
	}
	if (0) {
	AT24CXX_WRITE_BY_PAGE:
		if (WriteAddr % EE_PAGE_SIZE) {
			AT24CXX_WriteOneByte(WriteAddr,pBuffer);
			HAL_Delay(5);
			WriteAddr++;
			pBuffer++;
			NumToWrite--;
		}
		while (NumToWrite >= EE_PAGE_SIZE) {
			uint16_t page = WriteAddr / EE_PAGE_SIZE;
			AT24CXX_WritePage(page,pBuffer);
			HAL_Delay(5);
			WriteAddr += EE_PAGE_SIZE;
			pBuffer += EE_PAGE_SIZE;
			NumToWrite -= EE_PAGE_SIZE;
		}
		goto AT24CXX_WRITE_BY_BYTE;
	}
}

uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	AT24CXX_ReadOneByte(EE_TYPE-1,&temp);//避免每次开机都写AT24CXX			   
	if(temp==verify_Check_Byte)return 0;		   
	else//排除第一次初始化的情况
	{
		temp=verify_Check_Byte;
		AT24CXX_WriteOneByte(EE_TYPE-1,&temp);
		HAL_Delay(20);
	    AT24CXX_ReadOneByte(EE_TYPE-1,&temp);	  
		if(temp==verify_Check_Byte)return 0;
	}
	return 1;											  
}
 











