/*
 * button.h
 *
 *  Created on: 12 de ago de 2019
 *      Author: Joao Paulo
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "gos.h"
#include "common.h"
#include "nvm_settings.h"
#include "string.h"
#include "example_app_util.h"
#include <stdio.h>
#include "mqtt_demo_common.h"
#include "commands.h"

#define BUTTON_DEBOUNCE_TIME_MS  (10)   // ms
#define BUTTON_CLICK_TIME_MS     (500)  // ms
#define BUTTON_PRESS_TIME_MS     (1000) // ms
#ifndef WTOM01
	#define PULSE_DEBOUNCE_TIME_MS  (1)   // ms
	#define PULSE_CLICK_TIME_MS     (500)  // ms
//	#define PULSE_PRESS_TIME_MS     (1000) // ms
#endif

#define RESET_TIME (28500) //ms
#define PARAM_TIME (5500) //ms

#define TOGGLE_BUTTON_1 1
#define TOGGLE_BUTTON_2 2
#define PULSE_BUTTON 3

#define BUTTON_PLUS TOGGLE_BUTTON_2
#define BUTTON_MINUS TOGGLE_BUTTON_1


void button_init();

#endif /* BUTTON_H_ */
