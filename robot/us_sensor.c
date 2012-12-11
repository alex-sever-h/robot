
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

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

  // configure trigger pins
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
      GPIO12 | GPIO15);
  gpio_clear(GPIOB, GPIO12 | GPIO15);

  // configure echo pins
  gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
      GPIO0 | GPIO1);

  //enable einterrupts
  nvic_enable_irq(NVIC_EXTI0_IRQ);
  nvic_enable_irq(NVIC_EXTI1_IRQ);

  exti_select_source(EXTI0, GPIOC);
  exti_select_source(EXTI1, GPIOC);
  exti_set_trigger(EXTI0, EXTI_TRIGGER_BOTH);
  exti_set_trigger(EXTI1, EXTI_TRIGGER_BOTH);
  exti_enable_request(EXTI0);
  exti_enable_request(EXTI1);

}

void us_sensor_trigger(int select_dir)
{
  switch(select_dir)
  {
  case LEFT:{
    gpio_set(GPIOB, GPIO15);
    delay_us(10);
    gpio_clear(GPIOB, GPIO15);
  }break;
  case RIGHT:{
    gpio_set(GPIOB, GPIO12);
    delay_us(10);
    gpio_clear(GPIOB, GPIO12);
  }break;
  default:break;
  }
}


void check_us_sensor(int select_dir)
{
  int ctr;

  if ( gpio_get(GPIOC, GPIO0 | GPIO1) )
    {
      timer_set_counter(TIM2, 0);
      ctr = timer_get_counter(TIM2);
      bt_put((char)ctr);
    }
  else
    {
      u16 ctr;
      ctr = timer_get_counter(TIM2);
#if DEBUG

      char buffer[30];
      int_to_a(buffer, ctr);

      switch(select_dir)
      {
      case RIGHT:
        bt_puts("RIGHT: ");
        break;
      case LEFT:
        bt_puts("LEFT : ");
        break;
      default:break;
      }

      bt_puts(buffer);
      bt_puts("mm \n");
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

void exti0_isr(void)
{
  check_us_sensor(RIGHT);
  exti_reset_request(EXTI0);
}
void exti1_isr(void)
{
  check_us_sensor(LEFT);
  exti_reset_request(EXTI1);
}



