#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "motor.h"

// Pinos de controle do motor
#define EN_A 18
#define A1 16
#define A2 17

static eMotorRotate_t m_motor_rotate = eRotateNormal;
static uint16_t m_duty_cycle = 65535;

void motor_initialize()
{
  gpio_set_function(EN_A, GPIO_FUNC_PWM);
  int pwm_slice = pwm_gpio_to_slice_num(EN_A);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 4.0f);
  pwm_init(pwm_slice, &config, true);

  gpio_init(A1);
  gpio_init(A2);

  gpio_set_dir(A1, GPIO_OUT);
  gpio_set_dir(A2, GPIO_OUT);

  printf("Initializing Motor in Normal rotation and duty cycle in 100%\n");
  motor_set_rotation(eRotateNormal);
  motor_set_duty_cycle(100);
}

void motor_set_rotation(eMotorRotate_t rotation)
{
  m_motor_rotate = rotation;
  switch (m_motor_rotate)
  {
  case eRotateReverse:
    gpio_put(A1, false);
    gpio_put(A2, true);
    break;

  default:
    gpio_put(A1, true);
    gpio_put(A2, false);
    break;
  }
}

eMotorRotate_t motor_get_rotation() { return m_motor_rotate; }

void motor_set_duty_cycle(uint8_t value)
{
  uint8_t percentage = (value > 100) ? 100 : value;
  m_duty_cycle = ((float)((float)percentage / 100.0)) * 65535;
  printf("Now duty cycle is: %d\n", m_duty_cycle, percentage);
  pwm_set_gpio_level(EN_A, m_duty_cycle);
}
