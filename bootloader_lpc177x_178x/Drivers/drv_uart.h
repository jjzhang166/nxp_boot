#ifndef __DRV_UART_H
#define __DRV_UART_H

#include "lpc407x_8x_177x_8x.h"
#include "stdio.h"

#define IER_RBR     0x01
#define IER_THRE    0x02
#define IER_RLS     0x04

#define IIR_PEND    0x01
#define IIR_RLS     0x03
#define IIR_RDA     0x02
#define IIR_CTI     0x06
#define IIR_THRE    0x01

#define LSR_RDR     0x01
#define LSR_OE      0x02
#define LSR_PE      0x04
#define LSR_FE      0x08
#define LSR_BI      0x10
#define LSR_THRE    0x20
#define LSR_TEMT    0x40
#define LSR_RXFE    0x80

/*******************************************************************************************
* ��������UART0_Init()
* ����  ��void
* ����ֵ��void
* ����  ��UART0��ʼ����������ʹ��UART0ǰ�ȵ���
*********************************************************************************************/
void UART0_Init(uint32_t baudrate);
/*******************************************************************************************
* ��������UART0_SendByte()
* ����  ��uint8_t c  Ҫ�����ַ�
* ����ֵ��void
* ����  ��UART0����һ���ַ�����
*********************************************************************************************/
void UART0_SendByte(uint8_t c);


#endif
