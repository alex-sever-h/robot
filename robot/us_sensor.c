#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

#include "usart.h"
#include "generics.h"
#include "us_sensor.h"

t_sensor sensors[] =
{
		//front sensors
		{ .id = "FR_C", .echo_gpio = GPIOC, .echo_gpio_n = GPIO0,
				.exti = EXTI0, .trig_gpio = GPIOB,
				.trig_gpio_n = GPIO12, .distance = 0, },
				{ .id = "FR_L", .echo_gpio = GPIOC, .echo_gpio_n = GPIO1,
						.exti = EXTI1, .trig_gpio = GPIOC,
						.trig_gpio_n = GPIO12, .distance = 0, },
						{ .id = "FR_R", .echo_gpio = GPIOC, .echo_gpio_n = GPIO2,
								.exti = EXTI2, .trig_gpio = GPIOB,
								.trig_gpio_n = GPIO15, .distance = 0, },
								//rear sensors
								{ .id = "RE_C", .echo_gpio = GPIOC, .echo_gpio_n = GPIO3,
										.exti = EXTI3, .trig_gpio = GPIOB,
										.trig_gpio_n = GPIO12, .distance = 0, },
										{ .id = "RE_R", .echo_gpio = GPIOA, .echo_gpio_n = GPIO4,
												.exti = EXTI4, .trig_gpio = GPIOB,
												.trig_gpio_n = GPIO15, .distance = 0, },
												{ .id = "RE_L", .echo_gpio = GPIOA, .echo_gpio_n = GPIO5,
														.exti = EXTI5, .trig_gpio = GPIOC,
														.trig_gpio_n = GPIO12, .distance = 0, },
														//right sensors
														{ .id = "RI_C", .echo_gpio = GPIOA, .echo_gpio_n = GPIO6,
																.exti = EXTI6, .trig_gpio = GPIOB,
																.trig_gpio_n = GPIO12, .distance = 0, },
																{ .id = "RI_F", .echo_gpio = GPIOA, .echo_gpio_n = GPIO7,
																		.exti = EXTI7, .trig_gpio = GPIOC,
																		.trig_gpio_n = GPIO12, .distance = 0, },
																		{ .id = "RI_R", .echo_gpio = GPIOA, .echo_gpio_n = GPIO8,
																				.exti = EXTI8, .trig_gpio = GPIOB,
																				.trig_gpio_n = GPIO15, .distance = 0, },
																				//left sensors
																				{ .id = "LE_C", .echo_gpio = GPIOB, .echo_gpio_n = GPIO9,
																						.exti = EXTI9, .trig_gpio = GPIOB,
																						.trig_gpio_n = GPIO12, .distance = 0, },
																						{ .id = "LE_F", .echo_gpio = GPIOA, .echo_gpio_n = GPIO11,
																								.exti = EXTI11, .trig_gpio = GPIOC, .trig_gpio_n =
																										GPIO12, .distance = 0, },
																										{ .id = "LE_R", .echo_gpio = GPIOA, .echo_gpio_n = GPIO12,
																												.exti = EXTI12, .trig_gpio = GPIOB, .trig_gpio_n =
																														GPIO15, .distance = 0, }, };

void init_counter_sensor()
{
	uint32_t prescaler;

	prescaler = rcc_ppre2_frequency / 170000;

	timer_disable_counter(TIM2);
	timer_reset(TIM2);

	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_continuous_mode(TIM2);
	timer_set_prescaler(TIM2, prescaler);

	timer_enable_counter(TIM2);

}

void us_sensor_config(void)
{
	unsigned int i;
	init_counter_sensor();

	//enable almost all external interrupts
	nvic_enable_irq(NVIC_EXTI0_IRQ);
	nvic_enable_irq(NVIC_EXTI1_IRQ);
	nvic_enable_irq(NVIC_EXTI2_IRQ);
	nvic_enable_irq(NVIC_EXTI3_IRQ);
	nvic_enable_irq(NVIC_EXTI4_IRQ);
	nvic_enable_irq(NVIC_EXTI9_5_IRQ);
	nvic_enable_irq(NVIC_EXTI15_10_IRQ);
	nvic_set_priority(NVIC_EXTI0_IRQ, 255);
	nvic_set_priority(NVIC_EXTI1_IRQ, 255);
	nvic_set_priority(NVIC_EXTI2_IRQ, 255);
	nvic_set_priority(NVIC_EXTI3_IRQ, 255);
	nvic_set_priority(NVIC_EXTI4_IRQ, 255);
	nvic_set_priority(NVIC_EXTI9_5_IRQ, 255);
	nvic_set_priority(NVIC_EXTI15_10_IRQ, 255);

	for (i = 0; i < sizeof(sensors) / sizeof(t_sensor); ++i)
	{
		// configure trigger pin
		gpio_set_mode(sensors[i].trig_gpio, GPIO_MODE_OUTPUT_50_MHZ,
				GPIO_CNF_OUTPUT_PUSHPULL, sensors[i].trig_gpio_n);
		gpio_clear(sensors[i].trig_gpio, sensors[i].trig_gpio_n);

		// configure echo pins
		gpio_set_mode(sensors[i].echo_gpio, GPIO_MODE_INPUT,
				GPIO_CNF_INPUT_PULL_UPDOWN, sensors[i].echo_gpio_n);

		exti_select_source(sensors[i].exti, sensors[i].echo_gpio);
		exti_set_trigger(sensors[i].exti, EXTI_TRIGGER_RISING);
		exti_enable_request(sensors[i].exti);

		sensors[i].direction = EXTI_TRIGGER_RISING;
	}

}

void us_sensor_trigger()
{
	static int turn = 0;
	int i;

	if (turn == 0)
		timer_set_counter(TIM2, 0);

	gpio_set(sensors[turn].trig_gpio, sensors[turn].trig_gpio_n);
	delay_us(10);
	gpio_clear(sensors[turn].trig_gpio, sensors[turn].trig_gpio_n);

	turn = (turn + 1) % 3;

	for (i = turn; i < sizeof(sensors) / sizeof(t_sensor); i += 3)
	{
		exti_enable_request(sensors[i].exti);
	}
}

volatile u32 us_irq_timestamp;

void check_us_sensor()
{
	unsigned int i;
	u32 sensors_updated = 0;

	for (i = 0; i < sizeof(sensors) / sizeof(t_sensor); ++i)
	{
		if (exti_get_flag_status(sensors[i].exti))
		{
			//clear interrupt
			EXTI_PR = sensors[i].exti;

			//use interrupt
			switch (sensors[i].direction)
			{
			case EXTI_TRIGGER_RISING:
				sensors[i].timestamp = us_irq_timestamp;

				exti_set_trigger(sensors[i].exti, EXTI_TRIGGER_FALLING);
				sensors[i].direction = EXTI_TRIGGER_FALLING;

				break;
			case EXTI_TRIGGER_FALLING:
				sensors[i].distance = us_irq_timestamp - sensors[i].timestamp;
				sensors_updated |= (1 << i);

				exti_set_trigger(sensors[i].exti, EXTI_TRIGGER_RISING);
				sensors[i].direction = EXTI_TRIGGER_RISING;

				exti_disable_request(sensors[i].exti);
				break;
			default:
				break;
			}
		}
	}

	for (i = 0; i < sizeof(sensors) / sizeof(t_sensor); ++i)
	{
		if (sensors_updated & (1 << i))
		{
			char buffer[100];
			bt_puts(sensors[i].id);
			bt_puts(" : ");
			uint_to_a(buffer, (int) sensors[i].distance);
			bt_puts(buffer);
			bt_puts("mm\n");
		}
	}
}

void exti0_isr(void)
{
	us_irq_timestamp = timer_get_counter(TIM2);
	check_us_sensor();
}

void exti1_isr(void)
{
	us_irq_timestamp = timer_get_counter(TIM2);
	check_us_sensor();
}

void exti2_isr(void)
{
	us_irq_timestamp = timer_get_counter(TIM2);
	check_us_sensor();
}

void exti3_isr(void)
{
	us_irq_timestamp = timer_get_counter(TIM2);
	check_us_sensor();
}

void exti4_isr(void)
{
	us_irq_timestamp = timer_get_counter(TIM2);
	check_us_sensor();
}

void exti9_5_isr(void)
{
	us_irq_timestamp = timer_get_counter(TIM2);
	check_us_sensor();
}

void exti15_10_isr(void)
{
	us_irq_timestamp = timer_get_counter(TIM2);
	check_us_sensor();
}

