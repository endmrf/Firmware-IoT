/*
 * EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND
 * CONDITIONS OF THE CONTROLLING LICENSE AGREEMENT FOUND AT LICENSE.md
 * IN THIS SDK. IF YOU DO NOT AGREE TO THE LICENSE TERMS AND CONDITIONS,
 * PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.
 * (c) Copyright 2018, Silicon Laboratories Inc.  All rights reserved.
 */

#pragma once

#include "gos.h"
#include "mqtt.h"
#include "string.h"

#define MQTT_SETTINGS_MAGIC    0xAABBCCDD
//extern uint8_t* status;

// Settings struct for saving to NVM
typedef struct 
{
    uint32_t magic;          // Magic number to verify memory is initialized
    gos_mqtt_configuration_t configuration;
    gos_mqtt_will_t will;
    char will_message[MAX_WILL_MESSAGE_LENGTH];
    bool status;
    bool reset;
} mqtt_demo_settings_t;

typedef struct
{
	char* ssid;
	char* password;

}network_credentials_t;

typedef struct
{
	char password_device[40] ;
	char deviceID[40];
	char orgID[10];
	char brokerPort[5];

} device_configuration_t;

// Variables for CLI MQTT instance
extern gos_mqtt_handle_t mqtt_handle;
extern gos_mqtt_configuration_t* mqtt_configuration;
extern gos_mqtt_will_t* mqtt_will;
extern device_configuration_t device_configuration;
extern mqtt_demo_settings_t* settings;

