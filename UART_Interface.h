/*
 * URT_Interface.h
 *
 *  Created on: May 9, 2022
 *      Author: Ahmad
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

#define RX_BUFFER_SIZE	10
#define UBRR_VALUE	((F_CPU)/(16UL * BAUD_RATE)-1)

void UART_Init();
void UART_Send_Byte(u8 data);
void UART_Send_Array(u8 *data,u16 length);
void UART_Send_String(u8 *data);
u16 UART_Read_Count(void);
u8 UART_Read_Byte(void);


#endif /* UART_INTERFACE_H_ */
