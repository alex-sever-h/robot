
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include "usart.h"
#include "generics.h"
#include "us_sensor.h"

typedef enum{
	RISING,
	FALLING
} exti_t;


volatile unsigned int f_left_distance;
volatile unsigned int f_right_distance;
volatile unsigned int r_left_distance;
volatile unsigned int r_right_distance;

volatile exti_t us_sensor_direction_f_l;
volatile exti_t us_sensor_direction_f_r;
volatile exti_t us_sensor_direction_r_l;
volatile exti_t us_sensor_direction_r_r;

volatile u16 us_sensor_timestamp_f_l;
volatile u16 us_sensor_timestamp_f_r;
volatile u16 us_sensor_timestamp_r_l;
volatile u16 us_sensor_timestamp_r_r;


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
	init_counter_sensor();

	// configure trigger pin
	gpio_set_mode(US_SENSOR_GPIO, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
			US_SENSOR_TRIG);
	gpio_clear(US_SENSOR_GPIO, US_SENSOR_TRIG);

	// configure echo pins
	gpio_set_mode(US_SENSOR_GPIO, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
			US_SENSOR_ECHO);

	//enable einterrupts
	nvic_enable_irq(NVIC_EXTI9_5_IRQ);

	exti_select_source(EXTI5, US_SENSOR_GPIO);
	exti_select_source(EXTI6, US_SENSOR_GPIO);
	exti_select_source(EXTI7, US_SENSOR_GPIO);
	exti_select_source(EXTI8, US_SENSOR_GPIO);
	exti_set_trigger(EXTI5, EXTI_TRIGGER_RISING);
	exti_set_trigger(EXTI6, EXTI_TRIGGER_RISING);
	exti_set_trigger(EXTI7, EXTI_TRIGGER_RISING);
	exti_set_trigger(EXTI8, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI5);
	exti_enable_request(EXTI6);
	exti_enable_request(EXTI7);
	exti_enable_request(EXTI8);

	us_sensor_direction_f_l = RISING;
	us_sensor_direction_f_r = RISING;
	us_sensor_direction_r_l = RISING;
	us_sensor_direction_r_r = RISING;

}

void us_sensor_trigger()
{
	gpio_set(US_SENSOR_GPIO, US_SENSOR_TRIG);
	delay_us(10);
	exti_enable_request(EXTI5);
	exti_enable_request(EXTI6);
	exti_enable_request(EXTI7);
	exti_enable_request(EXTI8);
	gpio_clear(US_SENSOR_GPIO, US_SENSOR_TRIG);
	timer_set_counter(TIM2, 0);
}

volatile int up_fl = 0;
volatile int up_fr = 0;
volatile int up_rl = 0;
volatile int up_rr = 0;



void check_us_sensor()
{
	if(exti_get_flag_status(EXTI5))
	{
		EXTI_PR = EXTI5;
		switch(us_sensor_direction_f_l)
		{
		case RISING:
			//			if(gpio_get(US_SENSOR_GPIO, US_SENSOR_F_L) == 0)
			//				break;
			us_sensor_timestamp_f_l = timer_get_counter(TIM2);
			exti_set_trigger(EXTI5, EXTI_TRIGGER_FALLING);
			us_sensor_direction_f_l = FALLING;
			break;
		case FALLING:
			//			if(gpio_get(US_SENSOR_GPIO, US_SENSOR_F_L) != 0)
			//				break;
			f_left_distance = timer_get_counter(TIM2) - us_sensor_timestamp_f_l;
			exti_set_trigger(EXTI5, EXTI_TRIGGER_RISING);
			us_sensor_direction_f_l = RISING;
			up_fl++;
			exti_disable_request(EXTI5);
			break;
		default:
			break;
		}
		//		up_fl++;
		//		exti_reset_request(EXTI5);
	}

	if(exti_get_flag_status(EXTI6))
	{
		EXTI_PR = EXTI6;
		switch(us_sensor_direction_f_r)
		{
		case RISING:
			//			if(gpio_get(US_SENSOR_GPIO, US_SENSOR_F_R) == 0)
			//				break;
			us_sensor_timestamp_f_r = timer_get_counter(TIM2);
			exti_set_trigger(EXTI6, EXTI_TRIGGER_FALLING);
			us_sensor_direction_f_r = FALLING;
			break;
		case FALLING:
			//			if(gpio_get(US_SENSOR_GPIO, US_SENSOR_F_R) != 0)
			//				break;
			f_right_distance = timer_get_counter(TIM2) - us_sensor_timestamp_f_r;
			exti_set_trigger(EXTI6, EXTI_TRIGGER_RISING);
			us_sensor_direction_f_r = RISING;
			up_fr++;
			exti_disable_request(EXTI6);
			break;
		default:
			break;
		}
		//		up_fr++;
		//		exti_reset_request(EXTI6);
	}

	if(exti_get_flag_status(EXTI7))
	{
		EXTI_PR = EXTI7;
		switch(us_sensor_direction_r_l)
		{
		case RISING:
			//			if(gpio_get(US_SENSOR_GPIO, US_SENSOR_R_L) == 0)
			//				break;
			us_sensor_timestamp_r_l = timer_get_counter(TIM2);
			exti_set_trigger(EXTI7, EXTI_TRIGGER_FALLING);
			us_sensor_direction_r_l = FALLING;
			up_rl++;
			break;
		case FALLING:
			//			if(gpio_get(US_SENSOR_GPIO, US_SENSOR_R_L) != 0)
			//				break;
			r_left_distance = timer_get_counter(TIM2) - us_sensor_timestamp_r_l;
			exti_set_trigger(EXTI7, EXTI_TRIGGER_RISING);
			us_sensor_direction_r_l = RISING;
			exti_disable_request(EXTI7);
			break;
		default:
			break;
		}
		//		up_rl++;
		//		exti_reset_request(EXTI7);
	}

	if(exti_get_flag_status(EXTI8))
	{
		EXTI_PR = EXTI8;
		switch(us_sensor_direction_r_r)
		{
		case RISING:
			//			if(gpio_get(US_SENSOR_GPIO, US_SENSOR_R_R) == 0)
			//				break;
			us_sensor_timestamp_r_r = timer_get_counter(TIM2);
			exti_set_trigger(EXTI8, EXTI_TRIGGER_FALLING);
			us_sensor_direction_r_r = FALLING;
			break;
		case FALLING:
			//			if(gpio_get(US_SENSOR_GPIO, US_SENSOR_R_R) != 0)
			//				break;
			r_right_distance = timer_get_counter(TIM2) - us_sensor_timestamp_r_r;
			exti_set_trigger(EXTI8, EXTI_TRIGGER_RISING);
			us_sensor_direction_r_r = RISING;
			up_rr++;
			exti_disable_request(EXTI8);
			break;
		default:
			break;
		}
		//		up_rr++;
		//		exti_reset_request(EXTI8);
	}

}

void exti9_5_isr(void)
{
	check_us_sensor();
}



