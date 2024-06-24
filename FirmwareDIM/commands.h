
#ifndef COMMANDS_H_
#define COMMANDS_H_


#include "gos.h"
#include "common.h"
#include "mqtt.h"
#include "nvm_settings.h"
#include "string.h"
#include "example_app_util.h"
#include <stdio.h>
#include "mqtt_demo_common.h"
#include "connection.h"
#include "em_timer.h"
#include "em_cmu.h"

#define PUBLISH_ON_MESSAGE "{\"d\":{\"onOffStatus\":\"ON\"}}"
#define PUBLISH_OFF_MESSAGE "{\"d\":{\"onOffStatus\":\"OFF\"}}"
#define PUBLISH_UPDATING_MESSAGE "{\"d\":{\"firmwareUpdateStatus\":\"Updating\"}}"
#define ALLOW_MULTIPASS_UPDATES true
#define CAP_DELAY 2800 //us
#define PULSE_TIME 50

#define   DELAY_TIME_PULSE       1000    //us
#define   TIMER_PRESCALER        1
#define   TIMERn                 WTIMER1
#define   TIMERn_IRQ             WTIMER1_IRQn
#define   TIMERn_IRQ_HANDLER     WTIMER1_IRQHandler
#define   TIMERn_CMU_CLOCK       cmuClock_WTIMER1

//Aux Timer to start pulse
#define   TIMER3n                 WTIMER3
#define   TIMER3n_IRQ             WTIMER3_IRQn
#define   TIMER3n_IRQ_HANDLER     WTIMER3_IRQHandler
#define   TIMER3n_CMU_CLOCK       cmuClock_WTIMER3

//topics
#define ALL_TOPICS "iot-2/cmd/+/fmt/json"
#define PUBLISH_ONOFF_TOPIC "iot-2/evt/onOffStatus/fmt/json" //Publish topic ONOFF
#define PUBLISH_BRIGHTSET_TOPIC "iot-2/evt/brightSetStatus/fmt/json" //Publish topic ONOFF
#define PUBLISH_DATETIME_TOPIC "iot-2/evt/datetime-now/fmt/json" //Publish topic DateTime
#define PUBLISH_UPDATEFIRMWARE_TOPIC "iot-2/evt/firmwareUpdateStatus/fmt/json"

extern uint16_t dimming;
extern uint16_t level_dim[6];
extern uint32_t level_intensity[6];
extern bool up_down; //usado para indicar se o pulsador está aumentando ou diminuindo a intensidade do dimmer (true --> up; false --> down)
extern uint32_t timer_pulse;
extern uint64_t pressed_time;

void turn_on_off_device(bool status);
void receive_onoff_command (char* message_buffer);
void brightSet (char* message_buffer);
void datetime_update (char* message_buffer);
void reset_factory();
void parametrization_mode();
void stop_trigger();
void publish_bright();
void param_facReset ();
void change_dimming (uint32_t button);
void drive_triac();
void initTimer ();
void pulse_handler();
void removeChar(char *src, char *character);
void check_pulse ();

#endif /* COMMANDS_H_ */
