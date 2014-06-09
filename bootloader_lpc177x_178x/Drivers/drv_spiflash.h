#ifndef __SPIFLASH_H
#define __SPIFLASH_H

#include "lpc407x_8x_177x_8x.h"

#define SPI_FLASH_PageSize      256                                  //ҳ��С
#define SPI_FLASH_PerWritePageSize      256                          //ÿҳ��С

/*W25Xָ���*/
#define W25X_WriteEnable              0x06                            //дʹ��ָ��
#define W25X_WriteDisable             0x04                            //дʧ��ָ��
#define W25X_ReadStatusReg            0x05                            //��״̬�Ĵ���
#define W25X_WriteStatusReg           0x01                            //д״̬�Ĵ���
#define W25X_ReadData                 0x03                            //������ָ��
#define W25X_FastReadData             0x0B                            //���ٶ�ģʽָ��
#define W25X_FastReadDual             0x3B                            //���˫���ָ��    
#define W25X_PageProgram              0x02                            //ҳд��ָ��
#define W25X_BlockErase               0xD8                            //�����ָ��
#define W25X_SectorErase              0x20                            //��������ָ��
#define W25X_ChipErase                0xC7                            //Ƭ����ָ��
#define W25X_PowerDown                0xB9                            //�������ģʽָ��
#define W25X_ReleasePowerDown         0xAB                            //�˳�����ģʽ
#define W25X_DeviceID                 0xAB                            //��ȡоƬID
#define W25X_ManufactDeviceID         0x90                            //��ȡ����ID
#define W25X_JedecDeviceID            0x9F

#define WIP_Flag                      0x01                            //д��æ��־λ
#define Dummy_Byte                    0xFF                            //������

#define SPI_FLASH_CS_LOW()       LPC_GPIO0->CLR=(0x01<<16)    //ʹ��Ƭѡ
#define SPI_FLASH_CS_HIGH()      LPC_GPIO0->SET=(0x01<<16)       //ʧ��Ƭѡ

/******************************************************************************************
*��������SPI_FLASH_Init()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH��ʼ���������ⲿ����
*********************************************************************************************/
void SPI_FLASH_Init(void);
/******************************************************************************************
*��������SPI_FLASH_SectorErase()
* ������uint32_t SectorAddr   ���ַ
* ����ֵ��void
* ���ܣ�SPIFLASH���������������ⲿ����
*********************************************************************************************/
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
/******************************************************************************************
*��������SPI_FLASH_BulkErase()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH��Ƭ�����������ⲿ����
*********************************************************************************************/
void SPI_FLASH_BulkErase(void);
/******************************************************************************************
*��������SPI_FLASH_PageWrite()
* ������uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite ����ָ�룬д���ַ��д��ĸ���
* ����ֵ��void
* ���ܣ�SPIFLASHҳд�����ݺ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
/******************************************************************************************
*��������SPI_FLASH_BufferWrite()
* ������uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite ����ָ�룬д���ַ��д��ĸ���
* ����ֵ��void
* ���ܣ�SPIFLASH������ݺ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
/******************************************************************************************
*��������SPI_FLASH_BufferRead()
* ������uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead ����ָ�룬�����ĵ�ַ�������ĸ���
* ����ֵ��void
* ���ܣ�SPIFLASH������ݺ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
/******************************************************************************************
*��������SPI_FLASH_ReadID()
* ������void
* ����ֵ��uint32_t ����ID
* ���ܣ�SPIFLASH��ȡID�������ⲿ����
*********************************************************************************************/
uint32_t SPI_FLASH_ReadID(void);
/******************************************************************************************
*��������SPI_FLASH_ReadDeviceID()
* ������void
* ����ֵ��uint32_t �豸ID
* ���ܣ�SPIFLASH��ȡ�豸ID�������ⲿ����
*********************************************************************************************/
uint32_t SPI_FLASH_ReadDeviceID(void);
/******************************************************************************************
*��������SPI_FLASH_StartReadSequence()
* ������uint32_t ReadAddr 24λ����ַ
* ����ֵ��void
* ���ܣ�SPIFLASH����ʼ�������ⲿ����
*********************************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);

/******************************************************************************************
*��������SPI_FLASH_WriteEnable()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASHдʹ�ܺ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_WriteEnable(void);
/******************************************************************************************
*��������SPI_FLASH_WaitForWriteEnd()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH�ȴ�д��Ϻ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void);
/******************************************************************************************
*��������SPI_Flash_PowerDown()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH�������ģʽ�������ⲿ����
*********************************************************************************************/
void SPI_Flash_PowerDown(void);
/******************************************************************************************
*��������SPI_Flash_WAKEUP()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH���ѵ���ģʽ�������ⲿ����
*********************************************************************************************/
void SPI_Flash_WAKEUP(void);
#endif
