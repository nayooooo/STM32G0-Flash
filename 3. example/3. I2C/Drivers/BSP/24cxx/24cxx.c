#include "24cxx.h"

I2C_HandleTypeDef * dev_24cxx = &hi2c1;
static const uint32_t time_Out = 0XFFFF;
const uint8_t verify_Check_Byte = 0X55;

hw_err_t hw_device_at24cxx_register(hw_device_24cxx_t dev, const char *name, void *user_data)
{
	if (dev == HW_NULL)
		return -HW_ERROR;
	
	hw_device_register(&(dev->parent), name, HW_NULL);
	hw_device_add_method(&(dev->parent), "init",	(void(*)(void))AT24CXX_Init);
	hw_device_add_method(&(dev->parent), "open",	(void(*)(void))AT24CXX_Open);
	hw_device_add_method(&(dev->parent), "close",	(void(*)(void))AT24CXX_Close);
	hw_device_add_method(&(dev->parent), "read",	(void(*)(void))AT24CXX_Read);
	hw_device_add_method(&(dev->parent), "write",	(void(*)(void))AT24CXX_Write);
	hw_device_add_method(&(dev->parent), "control",	(void(*)(void))AT24CXX_Control);
	
	return HW_EOK;
}

hw_err_t AT24CXX_Init(hw_device_t dev)
{
	return HW_EOK;
}

hw_err_t AT24CXX_Open(hw_device_t dev, hw_uint16_t oflag)
{
	return HW_EOK;
}

hw_err_t AT24CXX_Close(hw_device_t dev)
{
	return HW_EOK;
}

hw_err_t AT24CXX_Control(hw_device_t dev, int cmd, void *args)
{
	if (cmd == 1) {
		if (hw_strncmp((const char *)args, "check", 5) == 0) {
			return AT24CXX_Check();
		}
	}
	return HW_EOK;
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

hw_size_t AT24CXX_Read(hw_device_t dev, hw_off_t ops, void *buffer, hw_size_t size)
{
	hw_size_t __size = size;
	
	if (size < EE_PAGE_SIZE) goto AT24CXX_READ_BY_BYTE;  // 待读出数据数目少于一页数据数目
	if (size - ops % EE_PAGE_SIZE < EE_PAGE_SIZE) goto AT24CXX_READ_BY_BYTE;
	else goto AT24CXX_READ_BY_PAGE;
	if (0) {
	AT24CXX_READ_BY_BYTE:
		while(size--)
		{
			AT24CXX_ReadOneByte(ops,buffer);
			ops++;
			buffer++;
		}
	}
	if (0) {
	AT24CXX_READ_BY_PAGE:
		if (ops % EE_PAGE_SIZE) {
			while (ops % EE_PAGE_SIZE) {
				AT24CXX_ReadOneByte(ops,buffer);
				ops++;
				buffer++;
				size--;
			}
		}
		while (size >= EE_PAGE_SIZE) {
			uint16_t page = ops / EE_PAGE_SIZE;
			AT24CXX_ReadPage(page,buffer);
			ops += EE_PAGE_SIZE;
			buffer += EE_PAGE_SIZE;
			size -= EE_PAGE_SIZE;
		}
		goto AT24CXX_READ_BY_BYTE;
	}
	
	return __size;
}

hw_size_t AT24CXX_Write(hw_device_t dev, hw_off_t ops, const void *buffer, hw_size_t size)
{
	hw_size_t __size = size;
	
	if (size < EE_PAGE_SIZE) goto AT24CXX_WRITE_BY_BYTE;  // 待写入数据数目少于一页数据数目
	if (size - ops % EE_PAGE_SIZE < EE_PAGE_SIZE) goto AT24CXX_WRITE_BY_BYTE;
	else goto AT24CXX_WRITE_BY_PAGE;
	if (0) {
	AT24CXX_WRITE_BY_BYTE:
		while(size--)
		{
			AT24CXX_WriteOneByte(ops,(uint8_t*)buffer);
			HAL_Delay(5);
			ops++;
			buffer++;
		}
	}
	if (0) {
	AT24CXX_WRITE_BY_PAGE:
		if (ops % EE_PAGE_SIZE) {
			AT24CXX_WriteOneByte(ops,(uint8_t*)buffer);
			HAL_Delay(5);
			ops++;
			buffer++;
			size--;
		}
		while (size >= EE_PAGE_SIZE) {
			uint16_t page = ops / EE_PAGE_SIZE;
			AT24CXX_WritePage(page,(uint8_t*)buffer);
			HAL_Delay(5);
			ops += EE_PAGE_SIZE;
			buffer += EE_PAGE_SIZE;
			size -= EE_PAGE_SIZE;
		}
		goto AT24CXX_WRITE_BY_BYTE;
	}
	
	return __size;
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
 











