/*
 * EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND
 * CONDITIONS OF THE CONTROLLING LICENSE AGREEMENT FOUND AT LICENSE.md
 * IN THIS SDK. IF YOU DO NOT AGREE TO THE LICENSE TERMS AND CONDITIONS,
 * PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.
 * (c) Copyright 2018, Silicon Laboratories Inc.  All rights reserved.
 */

#include "nvm_settings.h"
#include "mqtt.h"

#define DEFAULT_MQTT_CONFIGURATION \
{ \
   .mqtt_version = MQTT_VERSION_3_1_1, \
   .use_tls = true, \
   .clean_session = false, \
   .automatic_reconnect = false, \
   .network_interface = GOS_INTERFACE_WLAN, \
   .keep_alive_interval_seconds = 30, \
   .broker_port = 0, \
   .tx_buffer_size = 256, \
   .rx_buffer_size = 256, \
   .broker_hostname = "3a4ypl.messaging.internetofthings.ibmcloud.com", \
   .client_id = "", \
   .username = "use-token-auth", \
   .password = "" \
}

#define DEFAULT_MQTT_WILL \
{ \
   .qos_level = 1, \
   .retained = false, \
   .topic = "", \
   .payload = NULL, \
   .payload_length = 0 \
}

#define DEFAULT_MQTT_SETTINGS \
{ \
    .magic = MQTT_SETTINGS_MAGIC, \
    .configuration = DEFAULT_MQTT_CONFIGURATION, \
    .will = DEFAULT_MQTT_WILL, \
    .will_message ="", \
	.status = false \
}

// these values will automatically be loaded into NVM on startup
GOS_NVM_CREATE_DEFAULT_ENTRY(MQTT, DEMO, mqtt_demo_settings_t, DEFAULT_MQTT_SETTINGS);

//#define status_default 1

//GOS_NVM_CREATE_DEFAULT_ENTRY(STATUS, ONOFF, uint8_t, status_default);




