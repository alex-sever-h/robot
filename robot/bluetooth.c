

#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include "bluetooth.h"
#include "ringbuffer.h"

#define CONNECT     "CONNECT"
#define DISCONNECT  "DISCONNECT"

volatile RINGBUFFER u2rx, u2tx;

enum bt_mode_st
{
	CONNECTED,
	CMD_MODE
};

enum bt_mode_st bt_mode;


void bt_init( int baudrate )
{

	buffer_reset (&u2rx);
	buffer_reset (&u2tx);

	gpio_set_mode(GPIOB,
			GPIO_MODE_INPUT,
			GPIO_CNF_INPUT_FLOAT,
			GPIO_USART3_RX | GPIO_USART3_CTS);
	gpio_set_mode(GPIOB,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
			GPIO_USART3_TX | GPIO_USART3_RTS);

	nvic_enable_irq(NVIC_USART3_IRQ);

	//disable Transmit Data Register empty interrupt
	usart_enable_rx_interrupt(USART3);
	usart_disable_tx_interrupt(USART3);

	// UART peripheral configuration

	usart_set_baudrate(USART3, baudrate);
	usart_set_databits(USART3, 8);
	usart_set_parity(USART3, USART_PARITY_NONE);
	usart_set_mode(USART3, USART_MODE_RX | USART_MODE_TX);
	usart_set_stopbits(USART3, USART_STOPBITS_1);
	usart_set_flow_control(USART3, USART_FLOWCONTROL_RTS_CTS);

	usart_enable(USART3);

}


void usart3_isr(void)
{
	u8 ch;

	//if Receive interrupt
	if (((USART_CR1(USART3) & USART_CR1_RXNEIE) != 0) &&
			((USART_SR(USART3) & USART_SR_RXNE) != 0))
	{
		ch=usart_recv(USART3);
		buffer_put(&u2rx, ch);
	}

	if (((USART_CR1(USART3) & USART_CR1_TXEIE) != 0) &&
			((USART_SR(USART3) & USART_SR_TXE) != 0))
	{
		if (buffer_get(&u2tx, &ch) == SUCCESS)
		{
			//if char read from buffer
			usart_send(USART3, ch);
		}
		else	//if buffer empty
		{
			//disable Transmit Data Register empty interrupt
			usart_disable_tx_interrupt(USART3);
		}
	}
}

u8 bt_get_block(void)
{
	u8 ch;
	//check if buffer is empty
	while (buffer_empty(&u2rx) == SUCCESS);

	buffer_get(&u2rx, &ch);
	return ch;
}

u8 bt_get_noblock(void)
{
	u8 ch;
	//check if buffer is empty
	if (buffer_empty(&u2rx) == SUCCESS)
	{
		return 0;
	}else
	{
		buffer_get(&u2rx, &ch);
		return ch;
	}
}

u8 *bt_get_stream(void)
{
	if (buffer_empty(&u2rx) == SUCCESS)
		return 0;
	else
	{
		return u2rx.data;
	}
	return 0;
}

void bt_put(u8 ch)
{
	//put char to the buffer
	buffer_put(&u2tx, ch);
	//enable Transmit Data Register empty interrupt
	usart_enable_tx_interrupt(USART3);
}

void bt_puts(const u8 * string)
{
	while(*string)
	{
		buffer_put(&u2tx, *string);
		string++;
	}
	usart_enable_tx_interrupt(USART3);
}

int bt_connected_status()
{
	char buff[7+1];

	buffer_strncpy(&u2rx, buff, 7);
	buff[7] = '\0';
	usart_puts(buff);

	return 0;
}


void bt_send_sensor_reading(uint8_t sensor_code, unsigned int sensor_reading)
{
	bt_put(sensor_code);
	bt_put( (sensor_reading>> 0) & 0xFF);
	bt_put( (sensor_reading>> 8) & 0xFF);
	bt_put( (sensor_reading>>16) & 0xFF);
	bt_put( (sensor_reading>>24) & 0xFF);
	bt_put('\r');
}





