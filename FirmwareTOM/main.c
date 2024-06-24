#include "common.h"
#include "gos.h"
#include "mqtt.h"
#include "nvm_settings.h"
#include "string.h"
#include "example_app_util.h"
#include <stdio.h>
#include "mqtt_demo_common.h"
#include "commands.h"
#include "connection.h"
#include "button.h"


mqtt_demo_settings_t* settings;

#define MESSAGE_BUFFER_LENGTH       256


/*************************************************************************************************/
void gos_app_init(void)
{
	gos_result_t result;

	button_init();
	//gos_network_register_event_handler(GOS_INTERFACE_WLAN, (void *)connect_wlan_handler);
	//read last status of lamp before desenergization
	if (!GOS_FAILED(result, GOS_NVM_GET(MQTT, DEMO, settings)))
	{
		turn_on_off_device(settings->status);
	}
	
    if (GOS_FAILED(result, gos_load_app_settings("settings.ini")))
    {
        GOS_LOG("Failed to load settings, err:%d", result);
        return;
    }

    if (settings->configuration.broker_port != 0)
	{
		setup_mqtt(NULL, NULL, NULL, NULL);
	}


}
/*************************************************************************************************/

