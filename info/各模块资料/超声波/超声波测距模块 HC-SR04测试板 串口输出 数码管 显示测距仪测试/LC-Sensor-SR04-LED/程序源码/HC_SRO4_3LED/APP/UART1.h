#ifndef __UART1_H
#define __UART1_H

void Uart1_Init(void);
void UART1_SendByte(u8 data);
void UART1_SendString(u8 *s);
void UART1_ReturnString(void);
u8 UART1_ReceiveByte(void);
void Uart_Send(void);

#endif
