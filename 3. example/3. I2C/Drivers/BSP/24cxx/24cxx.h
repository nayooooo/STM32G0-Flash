#ifndef __24CXX_H
#define __24CXX_H
#include "i2c.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������
//24CXX���� ����(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

#define AT24C01				128
#define AT24C02				256
#define AT24C04				512
#define AT24C08				1024
#define AT24C16				2048
#define AT24C32				4096
#define AT24C64	    		8192
#define AT24C128			16384
#define AT24C256			32768
//Mini STM32������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
#define EE_TYPE 			AT24C02
#define EE_PAGE_SIZE		(8)
#define EE_PAGE_NUM		(EE_TYPE/EEPAGE_SIZE)

#define EE_ADDR			(0XA0)
#define EE_WRITE			(EE_ADDR | 0X00)
#define EE_READ			(EE_ADDR | 0X01)

void AT24CXX_ReadOneByte(uint16_t ReadAddr,uint8_t *DataToRead);
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t *DataToWrite);
void AT24CXX_ReadPage(uint16_t page,uint8_t *DataToReadBuff);
void AT24CXX_WritePage(uint16_t page,uint8_t *DataToWriteBuff);
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);

uint8_t AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC
#endif
















