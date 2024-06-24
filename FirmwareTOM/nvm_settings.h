/*
 * EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND
 * CONDITIONS OF THE CONTROLLING LICENSE AGREEMENT FOUND AT LICENSE.md
 * IN THIS SDK. IF YOU DO NOT AGREE TO THE LICENSE TERMS AND CONDITIONS,
 * PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.
 * (c) Copyright 2018, Silicon Laboratories Inc.  All rights reserved.
 */

#pragma once


#include "gos.h"
#include "mqtt_demo_common.h"

// define list of NVM entries for this app
// NOTE: For backwards compatibility, it's important the values
//       assigned to these entries do NOT change with new versions of your app.
//       To change or modify NVM entries, add a new entry with a new value to the list.
GOS_NVM_DEFINES_START

    GOS_NVM_DEFINE_TYPE(MQTT, 0),
        GOS_NVM_DEFINE_ENTRY(MQTT, DEMO, 0, sizeof(mqtt_demo_settings_t)),
	/*GOS_NVM_DEFINE_TYPE(STATUS, 1),
		GOS_NVM_DEFINE_ENTRY(STATUS, ONOFF, 0, sizeof(uint8_t)),*/
GOS_NVM_DEFINES_END
