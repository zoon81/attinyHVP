#include "uart.h"

void UARTInit()  
{
	UBRR0L = UBRR_VAL; 
	UBRR0H = (UBRR_VAL >> 8); 
	// UCSR0A |= (1<<U2X0);

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}
uint8_t  UARTReceive()
{
	while (!(UCSR0A & (1 << RXC0))){} 
	return UDR0;
}

void UARTSendByte(uint8_t data) 
{
	while (!(UCSR0A & (1 << UDRE0))){}
	UDR0 = data;
}

void UARTSendString(char * str)
{
	while (*str)
	{
		UARTSendByte(*str);
		str++;
	}
}
void UARTSend_uint8(uint8_t number)
{
	uint8_t data;
	if(number > 99){
		data = number / 100;
		number -= data * 100;
		UARTSendByte(data + 48);
		data = number / 10;
		number -= data * 10;
		UARTSendByte(data + 48);
		UARTSendByte(number + 48);
	}else if(number > 9){
		data = number / 10;
		number -= data * 10;
		UARTSendByte(data + 48);
		UARTSendByte(number + 48);
	} else if(number >= 0){
		UARTSendByte(number + 48);
	}
}

