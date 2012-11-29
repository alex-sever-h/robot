/*
 * uart.c
 *
 *  Created on: Apr 12, 2012
 *      Author: alexs
 */

#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include "usart.h"
#include "ringbuffer.h"

#define BUFFERED
#ifdef BUFFERED
volatile RINGBUFFER u1rx, u1tx;
#endif

void usart_init( int baudrate )
{
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

#ifdef BUFFERED


	buffer_reset (&u1rx);
	buffer_reset (&u1tx);

	nvic_enable_irq(NVIC_USART1_IRQ);
	usart_enable_rx_interrupt(USART1);
	usart_disable_tx_interrupt(USART1);

#endif

	// usart peripheral confguration
	usart_set_baudrate(USART1, baudrate);
	usart_set_databits(USART1, 8);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	usart_enable(USART1);

}


void usart1_isr(void)
{
	u8 ch;

	//if Receive interrupt
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
			((USART_SR(USART1) & USART_SR_RXNE) != 0))
	{
		ch=usart_recv(USART1);
		buffer_put(&u1rx, ch);
	}

	if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
			((USART_SR(USART1) & USART_SR_TXE) != 0))
	{
		if (buffer_get(&u1tx, &ch) == SUCCESS)
		{
			//if char read from buffer
			usart_send(USART1, ch);
		}
		else	//if buffer empty
		{

			//disable Transmit Data Register empty interrupt
			usart_disable_tx_interrupt(USART1);
		}
	}
}



void tty_put(u8 ch)
{
#ifdef BUFFERED
	//put char to the buffer
	buffer_put(&u1tx, ch);
	//enable Transmit Data Register empty interrupt
	usart_enable_tx_interrupt(USART1);
#else

	usart_send_blocking(USART1, ch);

#endif
}


u8 tty_get_noblock(void)
{
#ifdef BUFFERED
	u8 ch;
	//check if buffer is empty
	if (buffer_empty(&u1rx) == SUCCESS)
	{
		return 0;
	}
	else
	{
		buffer_get(&u1rx, &ch);
		return ch;
	}
#else
	return usart_recv_blocking(USART1);
#endif
}

u8 tty_rx_empty(void)
{
	return buffer_empty(&u1rx);
}


void usart_puts(const char * string)
{
#ifdef BUFFERED
	while(*string)
	{
		buffer_put(&u1tx, *string);
		usart_enable_tx_interrupt(USART1);
		string++;
	}
#else
	while(*string)
	{
		tty_put(*string);
		string++;
	}
#endif
}

















