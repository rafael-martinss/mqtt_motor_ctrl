
#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

typedef enum
{
    eRotateNormal,
    eRotateReverse,
} eMotorRotate_t;

void motor_initialize(void);
void motor_set_rotation(eMotorRotate_t rotation);
eMotorRotate_t motor_get_rotation();
void motor_set_duty_cycle(uint8_t value);

#endif /*MOTOR_H_*/
