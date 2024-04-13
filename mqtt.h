
#ifndef MQTT_H_
#define MQTT_H_

#include "motor.h"

int mqtt_initialize();

void mqtt_send_rotation(eMotorRotate_t rotation);
#endif /*MQTT_H_*/
