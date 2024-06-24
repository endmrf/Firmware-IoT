/*
 * connection.h
 *
 *  Created on: 9 de ago de 2019
 *      Author: Joao Paulo
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "common.h"
#include "gos.h"
#include "mqtt.h"
#include "nvm_settings.h"
#include "string.h"
#include "example_app_util.h"
#include <stdio.h>
#include "commands.h"
#include "button.h"
#include "mqtt_demo_common.h"

#define MESSAGE_BUFFER_LENGTH       256


void publish_handler(gos_mqtt_handle_t handle, gos_mqtt_message_t* message);
void setup_mqtt (char passAuth[], char orgID[], char deviceID[], char brokerPort[]);
//void setup_mqtt (char *passAuth, char *orgID, char *deviceID, char *brokerPort);
void check_network();
void check_connection_mqtt();
void set_ssid_name();
void load_pages();
void removeChar(char *src, char *character);
void publish_status(bool status);
void publish_firmwareStatus();
void connect_wlan_handler();
#endif /* CONNECTION_H_ */

extern gos_version_details_t version_firmware;
extern char version_str_buffer[100];
