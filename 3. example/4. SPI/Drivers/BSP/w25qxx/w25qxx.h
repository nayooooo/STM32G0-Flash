#ifndef __FLASH_H
#define __FLASH_H			    
#include "spi.h"
#include "device.h"
	  
//W25Xϵ��/Qϵ��оƬ�б�	   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

#define NM25Q80 	0X5213 	
#define NM25Q16 	0X5214
#define NM25Q32 	0X5215
#define NM25Q64 	0X5216
#define NM25Q128	0X5217
#define NM25Q256 	0X5218

extern hw_uint16_t W25QXX_TYPE;					//����W25QXXоƬ�ͺ�		   

#include "stm32g0xx_hal_gpio.h"
#define W25QXX_CS_ENA()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0)
#define W25QXX_CS_NENA()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1)
				 
////////////////////////////////////////////////////////////////////////////

//ָ���
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg		0x05
#define W25X_WriteStatusReg		0x01
#define W25X_ReadData			0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0x60
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F

void W25QXX_Init(void);
hw_uint16_t  W25QXX_ReadID(void);  	    		//��ȡFLASH ID
hw_uint8_t	 W25QXX_ReadSR(void);        		//��ȡ״̬�Ĵ��� 
void W25QXX_Write_SR(hw_uint8_t sr);  			//д״̬�Ĵ���
void W25QXX_Write_Enable(void);  		//дʹ�� 
void W25QXX_Write_Disable(void);		//д����
void W25QXX_Write_NoCheck(hw_uint8_t* pBuffer,hw_uint32_t WriteAddr,hw_uint16_t NumByteToWrite);
void W25QXX_Read(hw_uint8_t* pBuffer,hw_uint32_t ReadAddr,hw_uint16_t NumByteToRead);   //��ȡflash
void W25QXX_Write(hw_uint8_t* pBuffer,hw_uint32_t WriteAddr,hw_uint16_t NumByteToWrite);//д��flash
void W25QXX_Erase_Chip(void);    	  	//��Ƭ����
void W25QXX_Erase_Sector(hw_uint32_t Dst_Addr);	//��������
void W25QXX_Wait_Busy(void);           	//�ȴ�����
void W25QXX_PowerDown(void);        	//�������ģʽ
void W25QXX_WAKEUP(void);				//����
#endif
















