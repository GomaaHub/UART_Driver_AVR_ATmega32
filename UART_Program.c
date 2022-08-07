#include "Library/types.h"
#include "avr/interrupt.h"
#include "UART_Config.h"
#include "UART_Interface.h"
#include "UART_Private.h"


volatile u8 RX_BUFFER[RX_BUFFER_SIZE] = {0};
volatile static u16 RX_COUNT = 0;
volatile static u8 UART_TX_BUSY = 1;

void UART_Init(void)
{
	//u32 UBRR=0;

	//UBRR = (F_CPU/(speed * BAUD_RATE)) -1;		// calculate the UBRR value to be loaded into the UBRRL/UBRRH registers

	//UCSRC_REG &= ~(1 << URSEL);

	UBRRH_REG = (UBRR_VALUE >> 8) & 0x000F;			// Load bits number 8,9,10,11 of the UBRR value in the UBRRH register
	UBRRL_REG = (UBRR_VALUE & 0x00FF);				// Load the lower bits (7 to 0) of the UBRR value in the UBRRL register

	UCSRB_REG |= (1 << TXEN) | (1 << RXEN) | (1 << RXCIE) | (1 << TXCIE);	// Enable the Transmitter and the Receiver of the USART and their Complete Enable Interrupts
}

void UART_Send_Byte(u8 data)
{

	//while(((UCSRA_REG >> UDRE) & 1) !=1)  IF WORKING IN POLLING MODE

	while(UART_TX_BUSY==0);
	UART_TX_BUSY = 0;

	UDR_REG = data;
}

void UART_Send_Array(u8 *data,u16 length)
{
	for(u16 i=0;i<length;i++)
	{
		UART_Send_Byte(data[i]);
	}
}

void UART_Send_String(u8 *data)
{
	u16 i=0;

	do
	{
		UART_Send_Byte(data[i]);
		i++;
	}while(data[i] != '\0');

}

u16 UART_Read_Count(void)
{


	return RX_COUNT;
}


u8 UART_Read_Byte(void)
{
	static u16 RX_READ_POS = 0;
	u8 data = '\0';

	data = RX_BUFFER[RX_READ_POS];
	RX_READ_POS++;
	RX_COUNT--;

	if(RX_READ_POS >= RX_BUFFER_SIZE)
	{
		RX_READ_POS=0;
	}

	return data;
}

ISR(USART_RXC_vect)
{
	volatile static u16 RX_WRITE_POS = 0;

	RX_BUFFER[RX_WRITE_POS] = UDR_REG;
	RX_COUNT++;
	RX_WRITE_POS++;

	if(RX_WRITE_POS >= RX_BUFFER_SIZE)
	{
		RX_WRITE_POS=0;
	}

}


ISR(USART_TXC_vect)
{
	UART_TX_BUSY = 1;
}
