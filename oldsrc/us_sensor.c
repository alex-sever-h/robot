#include "usart.h"
#include "generics.h"


volatile unsigned int f_left_distance;
volatile unsigned int f_right_distance;


#define RIGHT 1
#define LEFT -1

#define DEBUG 1



void init_counter_sensor()
{
	uint32_t prescaler;


}



void us_sensor_config(void)
{

}

void us_sensor_trigger(int select_dir)
{

	switch(select_dir)
	{
	case LEFT:{

	}break;
	case RIGHT:{

	}break;
	default:break;
	}

}


void check_us_sensor(int select_dir)
{
	int ctr;

	if    ( )
	{
	}
	else
	{

#if DEBUG

		char buffer[30];
		int_to_a(buffer, ctr);

		switch(select_dir)
		{
		case RIGHT:
			usart_puts("RIGHT: ");
			break;
		case LEFT:
			usart_puts("LEFT : ");
			break;
		default:break;
		}

		usart_puts(buffer);
		usart_puts("mm \n");
#endif

		switch(select_dir)
		{
		case RIGHT:
			f_right_distance = ctr;
			break;
		case LEFT:
			f_left_distance = ctr;
			break;
		default:break;
		}

	}

}




