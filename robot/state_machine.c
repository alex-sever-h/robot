/*
 * state_machine.c
 *
 *  Created on: Nov 25, 2012
 *      Author: alexs
 */

#include <string.h>

#include "state_machine.h"
#include "bluetooth.h"
#include "usart.h"

volatile robot_state_t robot_state;

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
                      if (interpret_command(buffer) && robot_state == CONNECTED)
                        {
                          tty_puts("COMMAND: ");
                          tty_puts(buffer);
                          tty_puts("\r\n");
                        }
                    }
                }
            }
        }
      for(;robot_state == DISCONNECTED;)
        {
          //loopback BLUETOOTH <--> UART1 interface (ttyUSB)

          if (!tty_rx_empty())
            {
              c = tty_get_noblock();
              bt_put(c);
            }
          if(!bt_rx_empty())
            {
              c = bt_get_noblock();
              tty_put(c);

              //still check for commands as connected disconnected
              if (is_endchar(c))
                {
                  char buffer[128];
                  if (bt_get_command(buffer))
                    {
                      interpret_command(buffer);
                    }
                }
            }

        }
    }
}


int  state_connect(char *cmd_parameter)
{
  if(cmd_parameter[0] == '\'' && cmd_parameter[15] == '\'' &&
      cmd_parameter[5] == '-' && cmd_parameter[8] == '-')
    {
      robot_state = CONNECTED;
      tty_puts("\r\n **** connected to ");
      tty_puts(cmd_parameter);
      tty_puts("\r\n\r\n");
      return 1;
    }
  return 0;
}

int  state_disconnect(char *cmd_parameter)
{
  if(cmd_parameter[0] == '\'' && cmd_parameter[15] == '\'' &&
      cmd_parameter[5] == '-' && cmd_parameter[8] == '-')
    {
      robot_state = DISCONNECTED;
      tty_puts("\r\n **** disconnected from ");
      tty_puts(cmd_parameter);
      tty_puts("\r\n\r\n");
      return 1;
    }
  return 0;
}

int  is_connected()
{
	if(robot_state == CONNECTED)
		return 1;
	else
		return 0;
}
