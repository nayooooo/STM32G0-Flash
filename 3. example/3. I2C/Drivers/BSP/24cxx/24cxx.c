#include "24cxx.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//24CXX���� ����(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

I2C_HandleTypeDef * dev_24cxx = &hi2c1;
static const uint32_t time_Out = 0XFFFF;
const uint8_t verify_Check_Byte = 0X55;

//��ʼ��IIC�ӿ�
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
	if (NumToRead < EE_PAGE_SIZE) goto AT24CXX_READ_BY_BYTE;  // ������������Ŀ����һҳ������Ŀ
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
	if (NumToWrite < EE_PAGE_SIZE) goto AT24CXX_WRITE_BY_BYTE;  // ��д��������Ŀ����һҳ������Ŀ
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
	AT24CXX_ReadOneByte(EE_TYPE-1,&temp);//����ÿ�ο�����дAT24CXX			   
	if(temp==verify_Check_Byte)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		temp=verify_Check_Byte;
		AT24CXX_WriteOneByte(EE_TYPE-1,&temp);
		HAL_Delay(20);
	    AT24CXX_ReadOneByte(EE_TYPE-1,&temp);	  
		if(temp==verify_Check_Byte)return 0;
	}
	return 1;											  
}
 











