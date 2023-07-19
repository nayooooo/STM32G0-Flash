#include "w25qxx.h"
#include <stdio.h>
#include "delay.h"

hw_uint16_t W25QXX_TYPE = W25Q64;	// Ĭ����W25Q64
SPI_HandleTypeDef *dev_w25qxx = &hspi1;
static const uint32_t time_Out = 0XFFFF;

// 4KbytesΪһ��Sector
// 16������Ϊ1��Block
// W25Q64
// ����Ϊ8M�ֽ�,����128��Block,4096��Sector 

void W25QXX_Init(void)
{
	W25QXX_CS_NENA();
	W25QXX_TYPE = W25QXX_ReadID();  // ��ȡFLASH ID.
}

static void W25QXX_ReadOneByte(hw_uint8_t *data)
{
	HAL_SPI_Receive(dev_w25qxx, data, 1, time_Out);
}

static void W25QXX_WriteOneByte(const hw_uint8_t *data)
{
	HAL_SPI_Transmit(dev_w25qxx, (hw_uint8_t*)data, 1, time_Out);
}

// ��ȡW25QXX��״̬�Ĵ���
// BIT7  6   5   4   3   2   1   0
// SPR   RV  TB BP2 BP1 BP0 WEL BUSY
// SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
// TB,BP2,BP1,BP0:FLASH����д��������
// WEL:дʹ������
// BUSY:æ���λ(1,æ;0,����)
// Ĭ��:0x00
hw_uint8_t W25QXX_ReadSR(void)   
{
	hw_uint8_t byte = W25X_ReadStatusReg;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // ���Ͷ�ȡ״̬�Ĵ�������
	W25QXX_ReadOneByte(&byte);  // ��ȡһ���ֽ�
	W25QXX_CS_NENA();
	return byte;
}

// дW25QXX״̬�Ĵ���
// ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25QXX_Write_SR(hw_uint8_t sr)
{
	hw_uint8_t byte = W25X_ReadStatusReg;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // ���Ͷ�ȡ״̬�Ĵ�������
	W25QXX_WriteOneByte(&sr);    // д��һ���ֽ�
	W25QXX_CS_NENA();
}

// W25QXXдʹ��
// ��WEL��λ
void W25QXX_Write_Enable(void)
{
	hw_uint8_t byte = W25X_WriteEnable;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // ����дʹ��
	W25QXX_CS_NENA();
}

// W25QXXд��ֹ	
// ��WEL����  
void W25QXX_Write_Disable(void)   
{
	hw_uint8_t byte = W25X_WriteDisable;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // ����д��ָֹ��
	W25QXX_CS_NENA();
}

// ��ȡоƬID
// ����ֵ����:				   
// 0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
// 0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
// 0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
// 0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
// 0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
hw_uint16_t W25QXX_ReadID(void)
{
	hw_uint16_t Temp = 0;
	hw_uint8_t byte = W25X_ManufactDeviceID;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);  // ���Ͷ�ȡID����
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

// ��ȡSPI FLASH
// ��ָ����ַ��ʼ��ȡָ�����ȵ�����
// pBuffer:���ݴ洢��
// ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
// NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(hw_uint8_t* pBuffer,hw_uint32_t ReadAddr,hw_uint16_t NumByteToRead)
{
 	hw_uint16_t i;
	hw_uint8_t byte = W25X_ReadData;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);							// ���Ͷ�ȡ����
	byte = (hw_uint8_t)((ReadAddr)>>16);				// ����24bit��ַ
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)((ReadAddr)>>8);
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)ReadAddr;
	W25QXX_WriteOneByte(&byte);
    for(i = 0; i < NumByteToRead; i++)
	{
		W25QXX_ReadOneByte(&pBuffer[i]);  // ѭ������
    }
	W25QXX_CS_NENA();
}

// SPI��һҳ(0~65535)��д������256���ֽڵ�����
// ��ָ����ַ��ʼд�����256�ֽڵ�����
// pBuffer:���ݴ洢��
// WriteAddr:��ʼд��ĵ�ַ(24bit)
// NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(hw_uint8_t* pBuffer,hw_uint32_t WriteAddr,hw_uint16_t NumByteToWrite)
{
 	hw_uint16_t i;
    W25QXX_Write_Enable();                  	// SET WEL
	W25QXX_CS_ENA();
	hw_uint8_t byte = W25X_PageProgram;
	W25QXX_WriteOneByte(&byte);      	// ����дҳ����
	byte = (hw_uint8_t)((WriteAddr)>>16);				// ����24bit��ַ
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)((WriteAddr)>>8);
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)WriteAddr;
	W25QXX_WriteOneByte(&byte);
    for(i = 0; i < NumByteToWrite; i++)
	{
		W25QXX_WriteOneByte(&pBuffer[i]);  // ѭ��д��
	}
	W25QXX_CS_NENA();
	W25QXX_Wait_Busy();					   		// �ȴ�д�����
}

// �޼���дSPI FLASH 
// ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
// �����Զ���ҳ���� 
// ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
// pBuffer:���ݴ洢��
// WriteAddr:��ʼд��ĵ�ַ(24bit)
// NumByteToWrite:Ҫд����ֽ���(���65535)
// CHECK OK
void W25QXX_Write_NoCheck(hw_uint8_t* pBuffer, hw_uint32_t WriteAddr, hw_uint16_t NumByteToWrite)
{
	hw_uint16_t pageremain;
	pageremain = 256 - WriteAddr % 256;  // ��ҳʣ����ֽ���
	if(NumByteToWrite <= pageremain)  // ������256���ֽ�
		pageremain = NumByteToWrite;
	while(1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if(NumByteToWrite == pageremain) break;  // д�������
	 	else  // NumByteToWrite > pageremain
		{
			pBuffer += pageremain;		// ������д�������
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain;			  // ��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)  // һ�ο���д��256���ֽ�
				pageremain = 256;
			else  // ����256���ֽ���
				pageremain = NumByteToWrite;
		}
	};
}

// дSPI FLASH  
// ��ָ����ַ��ʼд��ָ�����ȵ�����
// �ú�������������!
// pBuffer:���ݴ洢��
// WriteAddr:��ʼд��ĵ�ַ(24bit)						
// NumByteToWrite:Ҫд����ֽ���(���65535)   
hw_uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(hw_uint8_t* pBuffer, hw_uint32_t WriteAddr, hw_uint16_t NumByteToWrite)   
{ 
	hw_uint32_t secpos;
	hw_uint16_t secoff;
	hw_uint16_t secremain;	   
 	hw_uint16_t i;    
	hw_uint8_t * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);		//�����������
			for(i=0;i<secremain;i++)	   		//����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  				//ָ��ƫ��
			WriteAddr+=secremain;				//д��ַƫ��	   
		   	NumByteToWrite-=secremain;			//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;//��һ����������д����
			else secremain=NumByteToWrite;		//��һ����������д����
		}	 
	};
}

// ��������оƬ		  
// �ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)   
{
    W25QXX_Write_Enable();                 	 	// SET WEL
    W25QXX_Wait_Busy();
	hw_uint8_t byte = W25X_ChipErase;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);					// ����Ƭ��������
	W25QXX_CS_NENA();
	W25QXX_Wait_Busy();   				   		// �ȴ�оƬ��������
}

// ����һ������
// Dst_Addr:������ַ ����ʵ����������
// ����һ��ɽ��������ʱ��:150ms
void W25QXX_Erase_Sector(hw_uint32_t Dst_Addr)   
{
	//����falsh�������,������
 	printf("fe:%x\r\n", Dst_Addr);
 	Dst_Addr *= 4096;
    W25QXX_Write_Enable();                  	// SET WEL
    W25QXX_Wait_Busy();
	hw_uint8_t byte = W25X_SectorErase;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);					// ������������ָ��
	byte = (hw_uint8_t)((Dst_Addr)>>16);		// ����24bit��ַ
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)((Dst_Addr)>>8);
	W25QXX_WriteOneByte(&byte);
	byte = (hw_uint8_t)Dst_Addr;
	W25QXX_WriteOneByte(&byte);
	W25QXX_CS_NENA();
    W25QXX_Wait_Busy();   				   		// �ȴ��������
}

// �ȴ�����
void W25QXX_Wait_Busy(void)
{
	while((W25QXX_ReadSR()&0x01) == 0x01) HAL_Delay(5);  		// �ȴ�BUSYλ���
}

// �������ģʽ
void W25QXX_PowerDown(void)
{
	hw_uint8_t byte = W25X_PowerDown;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);					//���͵�������
	W25QXX_CS_NENA();
    delay_us(3);								//�ȴ�TPD
}

// ����
void W25QXX_WAKEUP(void)
{
	hw_uint8_t byte = W25X_ReleasePowerDown;
	W25QXX_CS_ENA();
	W25QXX_WriteOneByte(&byte);					//  send W25X_PowerDown command 0xAB
	W25QXX_CS_NENA();
    delay_us(3);                            	//�ȴ�TRES1
}


























