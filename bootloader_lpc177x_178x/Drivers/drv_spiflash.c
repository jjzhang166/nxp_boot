#include "drv_spiflash.h"
#include "drv_spi.h"

/******************************************************************************************
*��������SPI_FLASH_Init()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH��ʼ���������ⲿ����
*********************************************************************************************/
void SPI_FLASH_Init(void)
{
	      LPC_IOCON->P0_16 = 0;
        LPC_GPIO0->DIR|=(0x01<<16);
			  LPC_GPIO0->SET|=(0x01<<16);
        SSP0_Init(5000000);
}
/******************************************************************************************
*��������SPI_FLASH_SectorErase()
* ������uint32_t SectorAddr   ���ַ
* ����ֵ��void
* ���ܣ�SPIFLASH���������������ⲿ����
*********************************************************************************************/
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /*����д����ʹ��ָ��*/
  SPI_FLASH_WriteEnable();
	/*�ȴ�����д�꣬��֤д�����ǿ��е�*/
  SPI_FLASH_WaitForWriteEnd();
  /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();
  /*������������ָ��*/
  SSP0_SendByte(W25X_SectorErase);
  /*���Ϳ��ַ��8λ*/
  SSP0_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*���Ϳ��ַ��8λ*/
  SSP0_SendByte((SectorAddr & 0xFF00) >> 8);
  /*���Ϳ��ַ��8λ*/
  SSP0_SendByte(SectorAddr & 0xFF);
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();
  /* �ȴ�д���*/
  SPI_FLASH_WaitForWriteEnd();
}

/******************************************************************************************
*��������SPI_FLASH_BulkErase()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH��Ƭ�����������ⲿ����
*********************************************************************************************/
void SPI_FLASH_BulkErase(void)
{
  /*ʹ��д��*/
  SPI_FLASH_WriteEnable();
   /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();
  /*������Ƭ����ָ��*/
  SSP0_SendByte(W25X_ChipErase);
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();
  /* �ȴ�д���*/
  SPI_FLASH_WaitForWriteEnd();
}

/******************************************************************************************
*��������SPI_FLASH_PageWrite()
* ������uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite ����ָ�룬д���ַ��д��ĸ���
* ����ֵ��void
* ���ܣ�SPIFLASHҳд�����ݺ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
   /*ʹ��д��*/
  SPI_FLASH_WriteEnable();
  /*ʹ��Ƭѡ*/
  SPI_FLASH_CS_LOW();
  /* ����ҳд��ָ��*/
  SSP0_SendByte(W25X_PageProgram);
  /*���͸�8λ���ݵ�ַ*/
  SSP0_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*������8λ���ݵ�ַ*/
  SSP0_SendByte((WriteAddr & 0xFF00) >> 8);
  /*���͵�8λ���ݵ�ַ*/
  SSP0_SendByte(WriteAddr & 0xFF);
  /*���д��������Ƿ񳬳�ҳ��������С*/
  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
  }
  /*ѭ��д������*/
  while (NumByteToWrite--)
  {
    /*��������*/
    SSP0_SendByte(*pBuffer);
    /* ָ���Ƶ���һ��д������ */
    pBuffer++;
  }
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();
  /* �ȴ�д���*/
  SPI_FLASH_WaitForWriteEnd();
}

/******************************************************************************************
*��������SPI_FLASH_BufferWrite()
* ������uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite ����ָ�룬д���ַ��д��ĸ���
* ����ֵ��void
* ���ܣ�SPIFLASH������ݺ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
  Addr = WriteAddr % SPI_FLASH_PageSize;                           //����д���ҳ�Ķ�Ӧ��ʼ��ַ
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;                //�����ܹ�Ҫд��ҳ��
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;               //����ʣ�൥��ҳд�����ݸ���
  if (Addr == 0) /* ���Ҫд���ҳ��ַΪ0��˵��������ҳд���ݣ�û��ƫ��*/
  {
    if (NumOfPage == 0) /* ��������д��ҳ��Ϊ0��˵����������һ��ҳ�ķ�Χ�ڣ���ֱ�ӽ���ҳ��д*/
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);      //����ҳд����
    }
    else /* ���Ҫд��ҳ������0*/
    { 
			/*�Ƚ���ͷ���ݽ�����ҳд��*/
      while (NumOfPage--)
      { 
				//��ҳд��
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				//��ַƫ��
        WriteAddr +=  SPI_FLASH_PageSize;
				//����ָ��ƫ��
        pBuffer += SPI_FLASH_PageSize;
      }
       //��ʣ�����ݸ���д��
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*���д��ĵ�ַ����ҳ�Ŀ�ͷλ��*/
  {
    if (NumOfPage == 0) /*���д������ҳ�ĸ���Ϊ0��������С��һҳ����*/
    {
      if (NumOfSingle > count) /*���ʣ�����ݴ��ڵ�ǰҳ��ʣ������*/
      {
        temp = NumOfSingle - count;     //���㳬�������ݸ���
				/*д����ǰҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				/*���õ�ַƫ��*/
        WriteAddr +=  count;
				/*��������ָ��ƫ��*/
        pBuffer += count;
        /*��ʣ����д���µ�ҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else  /*���ʣ������С�ڵ�ǰҳ��ʣ������*/
      {
				/*ֱ��д�뵱ǰҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*���д������ҳ�ĸ�������0�������ݴ���һҳ����*/
    {
      NumByteToWrite -= count;         //�����ݼ�ȥ��ǰҳʣ�������
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;  //����Ҫд����ҳ����
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize; //����ʣ�����ݸ���
      /*����ͷ����д�뵱ǰҳʣ���ֽڸ���*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			/*���õ�ַƫ��*/
      WriteAddr +=  count;
			/*��������ָ��ƫ��*/
      pBuffer += count;
       /*��ʼʣ�����ݵ���ҳд��*/
      while (NumOfPage--)
      {
				/*д��һ��ҳ���ֽ���*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				/*���õ�ַƫ��*/
        WriteAddr +=  SPI_FLASH_PageSize;
				/*����ָ��ƫ��*/
        pBuffer += SPI_FLASH_PageSize;
      }
      /*���ʣ�����ݴ���0����ʣ��ĸ���д����һ��ҳ*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/******************************************************************************************
*��������SPI_FLASH_BufferRead()
* ������uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead ����ָ�룬�����ĵ�ַ�������ĸ���
* ����ֵ��void
* ���ܣ�SPIFLASH������ݺ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
   /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();
  /*���Ͷ�����ָ��*/
  SSP0_SendByte(W25X_ReadData);
  /*����24λ���ݵ�ַ*/
  /* ���͸�8λ���ݵ�ַ*/
  SSP0_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*������8λ���ݵ�ַ*/
  SSP0_SendByte((ReadAddr& 0xFF00) >> 8);
  /*���͵�8λ���ݵ�ַ*/
  SSP0_SendByte(ReadAddr & 0xFF);
  while (NumByteToRead--) /* ѭ����ȡ����*/
  {
    /*��ȡһ���ֽ�����*/
    *pBuffer = SSP0_SendByte(Dummy_Byte);
    /*����ָ���1*/
    pBuffer++;
  }
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();
}

/******************************************************************************************
*��������SPI_FLASH_ReadID()
* ������void
* ����ֵ��uint32_t ����ID
* ���ܣ�SPIFLASH��ȡID�������ⲿ����
*********************************************************************************************/
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();

  /*����ʶ������ID��*/
  SSP0_SendByte(W25X_JedecDeviceID);
  /* ��ȡһ���ֽ�*/
  Temp0 = SSP0_SendByte(Dummy_Byte);
  /* ��ȡһ���ֽ�*/
  Temp1 = SSP0_SendByte(Dummy_Byte);
   /* ��ȡһ���ֽ�*/
  Temp2 = SSP0_SendByte(Dummy_Byte);
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
  return Temp;
}
/******************************************************************************************
*��������SPI_FLASH_ReadDeviceID()
* ������void
* ����ֵ��uint32_t �豸ID
* ���ܣ�SPIFLASH��ȡ�豸ID�������ⲿ����
*********************************************************************************************/
uint32_t SPI_FLASH_ReadDeviceID(void)
{
  uint32_t Temp = 0;
   /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();
  /*���Ͷ�ȡIDָ��*/
  SSP0_SendByte(W25X_DeviceID);
  SSP0_SendByte(Dummy_Byte);
  SSP0_SendByte(Dummy_Byte);
  SSP0_SendByte(Dummy_Byte);
  /*��ȡ8λ����*/
  Temp = SSP0_SendByte(Dummy_Byte);
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();
  return Temp;
}
/******************************************************************************************
*��������SPI_FLASH_StartReadSequence()
* ������uint32_t ReadAddr 24λ����ַ
* ����ֵ��void
* ���ܣ�SPIFLASH����ʼ�������ⲿ����
*********************************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
   /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();
  /* ���Ͷ�����ָ��*/
  SSP0_SendByte(W25X_ReadData);
  /*����24λ���ݵ�ַ*/
  /* ���͸�8λ���ݵ�ַ */
  SSP0_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*������8λ���ݵ�ַ*/
  SSP0_SendByte((ReadAddr& 0xFF00) >> 8);
  /*���͵�8λ���ݵ�ַ*/
  SSP0_SendByte(ReadAddr & 0xFF);
}

/******************************************************************************************
*��������SPI_FLASH_WriteEnable()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASHдʹ�ܺ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
   /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();
  /*����дʹ��ָ��*/
  SSP0_SendByte(W25X_WriteEnable);
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();
}

/******************************************************************************************
*��������SPI_FLASH_WaitForWriteEnd()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH�ȴ�д��Ϻ������ⲿ����
*********************************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;
   /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();
  /*���Ͷ�״ָ̬�� */
  SSP0_SendByte(W25X_ReadStatusReg);
  /*ѭ�����Ϳ�����ֱ��FLASHоƬ����*/
  do
  {
    /* ���Ϳ��ֽ� */
    FLASH_Status = SSP0_SendByte(Dummy_Byte);
  }
  while ((FLASH_Status & WIP_Flag) != 0); /* ����Ƿ����*/
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();
}

/******************************************************************************************
*��������SPI_Flash_PowerDown()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH�������ģʽ�������ⲿ����
*********************************************************************************************/
void SPI_Flash_PowerDown(void)   
{ 
  /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();
  /*���͵���ָ�� */
  SSP0_SendByte(W25X_PowerDown);
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();
}   

/******************************************************************************************
*��������SPI_Flash_WAKEUP()
* ������void
* ����ֵ��void
* ���ܣ�SPIFLASH���ѵ���ģʽ�������ⲿ����
*********************************************************************************************/
void SPI_Flash_WAKEUP(void)   
{
  /* ʹ��Ƭѡ */
  SPI_FLASH_CS_LOW();
  /* �����˳�����ģʽָ�� */
  SSP0_SendByte(W25X_ReleasePowerDown);
  /*ʧ��Ƭѡ*/
  SPI_FLASH_CS_HIGH();              
}  
