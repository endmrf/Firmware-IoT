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

#define READ_POST_DATA_TIMEOUT 		1000
#define MESSAGE_BUFFER_LENGTH       256

GOS_HTTP_SERVER_DYNAMIC_PAGES_START
    GOS_HTTP_SERVER_DYNAMIC_PAGE("/json_parser",    json_parser_request),
	GOS_HTTP_SERVER_DYNAMIC_PAGE("/get_type",    get_type_request),
GOS_HTTP_SERVER_DYNAMIC_PAGES_END

void publish_handler(gos_mqtt_handle_t handle, gos_mqtt_message_t* message);
void setup_mqtt (const gos_json_tok_t *passAuth, const gos_json_tok_t *orgID, const gos_json_tok_t *deviceID, const gos_json_tok_t *brokerPort);
void check_network();
void check_connection_mqtt();
void set_ssid_name();
void load_pages();
void publish_status (bool status);
void publish_bright (char* brightness);
void publish_firmwareStatus();
void connect_wlan_handler();
#endif /* CONNECTION_H_ */

extern gos_mqtt_configuration_t* mqtt_configuration;
extern gos_mqtt_will_t* mqtt_will;
extern gos_mqtt_handle_t mqtt_handle;
extern gos_version_details_t version_firmware;
extern char version_str_buffer[100];
