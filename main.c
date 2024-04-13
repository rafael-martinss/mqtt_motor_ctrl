/**
 * @author Rafael Martins de Sousa - RA: 220023201
 * @brief Arquivo referente à "Atividade 02 - Acionando Motores através da internet"
 */

#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "bsp/board.h"

#include "motor.h"
#include "mqtt.h"

int main()
{
  stdio_init_all();
  printf("Initializing..\n");

  if (mqtt_initialize() != 0)
  {
    return 1;
  }

  motor_initialize();
  while (1)
  {
    if (board_button_read())
    {
      motor_set_rotation(eRotateReverse);
    }
    else
    {
      motor_set_rotation(eRotateNormal);
    }

    sleep_ms(100);
  }

  return 0;
}
