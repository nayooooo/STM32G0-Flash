#ifndef __24CXX_H
#define __24CXX_H
#include "device.h"
#include "i2c.h"

struct hw_device_24cxx
{
	struct hw_device			parent;
};
typedef struct hw_device_24cxx *hw_device_24cxx_t;

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
#define EE_PAGE_NUM			(EE_TYPE/EEPAGE_SIZE)

#define EE_ADDR				(0XA0)
#define EE_WRITE			(EE_ADDR | 0X00)
#define EE_READ				(EE_ADDR | 0X01)

void AT24CXX_ReadOneByte(uint16_t ReadAddr,uint8_t *DataToRead);
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t *DataToWrite);
void AT24CXX_ReadPage(uint16_t page,uint8_t *DataToReadBuff);
void AT24CXX_WritePage(uint16_t page,uint8_t *DataToWriteBuff);
hw_size_t AT24CXX_Write(hw_device_t dev, hw_off_t ops, const void *buffer, hw_size_t size);
hw_size_t AT24CXX_Read(hw_device_t dev, hw_off_t ops, void *buffer, hw_size_t size);

hw_err_t hw_device_at24cxx_register(hw_device_24cxx_t dev, const char *name, void *user_data);
uint8_t AT24CXX_Check(void);  //�������
hw_err_t AT24CXX_Init(hw_device_t dev); //��ʼ��IIC
hw_err_t AT24CXX_Open(hw_device_t dev, hw_uint16_t oflag);
hw_err_t AT24CXX_Close(hw_device_t dev);
hw_err_t AT24CXX_Control(hw_device_t dev, int cmd, void *args);
#endif
















