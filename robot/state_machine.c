/*
 * state_machine.c
 *
 *  Created on: Nov 25, 2012
 *      Author: alexs
 */

#include "state_machine.h"
#include "bluetooth.h"
#include "usart.h"

robot_state_t robot_state;

void wait_next_cmd(void)
{


}

void loop_states(void)
{
	char c;

	for(;;)
	{
		for(;robot_state == CONNECTED;)
		{
			//wait for commands
			if(!bt_rx_empty())
			{
				c = bt_get_block();
				if (is_endchar(c))
				{
					char buffer[128];
					if (bt_get_command(buffer))
					{
						// interpret command

						usart_puts("COMMAND: ");
						usart_puts(buffer);
						tty_put('\r');
					}
				}
			}
		}
		for(;robot_state == DISCONNECTED;)
		{
			//loopback bt UART through UART1 interface (ttyUSB)

			if (!tty_rx_empty())
			{
				c = tty_get_noblock();
				bt_put(c);
			}
			if(!bt_rx_empty())
			{
				c = bt_get_noblock();
				tty_put(c);
			}

		}
	}
}


int  state_connect(void)
{
	robot_state = CONNECTED;
	return 0;
}
int  state_disconnect(void)
{
	robot_state = DISCONNECTED;
	return 0;
}
