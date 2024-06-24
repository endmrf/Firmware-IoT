
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


#define PUBLISH_ON_MESSAGE "{\"d\":{\"onOffStatus\":\"ON\"}}"
#define PUBLISH_OFF_MESSAGE "{\"d\":{\"onOffStatus\":\"OFF\"}}"
#define PUBLISH_UPDATING_MESSAGE "{\"d\":{\"firmwareUpdateStatus\":\"Updating\"}}"
#define ALLOW_MULTIPASS_UPDATES true
#define BLINK_PERIOD_MS 300
#define COUNT_PERIOD_MS 100

//topics
#define ALL_TOPICS "iot-2/cmd/+/fmt/json"
#define PUBLISH_ONOFF_TOPIC "iot-2/evt/onOffStatus/fmt/json" //Publish topic ONOFF
#define PUBLISH_DATETIME_TOPIC "iot-2/evt/datetime-now/fmt/json" //Publish topic DateTime
#define PUBLISH_UPDATEFIRMWARE_TOPIC "iot-2/evt/firmwareUpdateStatus/fmt/json"



void turn_on_off_device(bool status);
void receive_onoff_command (char* message_buffer);
void datetime_update (char* message_buffer);
void reset_factory();
void parametrization_mode();
void blink_event_handler();
void counter_time();
#endif /* COMMANDS_H_ */
