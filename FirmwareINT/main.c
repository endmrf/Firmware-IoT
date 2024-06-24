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
gos_thread_t toggle_thread;


gos_thread_config_t config_thread =
{
		.function = toggle_handler,
		.stack_size = 4096,
		.stack_buffer = 0,
		.arg = 0,
		.priority = GOS_THREAD_PRIORITY_HIGHER
};

//static gos_system_monitor_t system_monitor;

#define MESSAGE_BUFFER_LENGTH       256
//#define MAX_DELAY                (30000)



/*************************************************************************************************/
void gos_app_init(void)
{
	gos_result_t result;

	button_init();
	//read last status of lamp before desenergization
	if (!GOS_FAILED(result, GOS_NVM_GET(MQTT, DEMO, settings)))
	{
		if(!settings->reset && !toggle && gos_system_get_faults_count() != 1)
			settings->status = !settings->status;

		turn_on_off_device(settings->status);
	}

	gos_rtos_thread_create(&toggle_thread, &config_thread);

    if (GOS_FAILED(result, gos_load_app_settings("settings.ini")))
    {
        GOS_LOG("Failed to load settings, err:%d", result);
        return;
    }

    //if connected successfully on Wifi Network and get the mqtt config from memory
    if (settings->configuration.broker_port != 0)
	{
		setup_mqtt(NULL, NULL, NULL, NULL);
	}

    settings->reset = false;
    gos_nvm_save();

}
/*************************************************************************************************/

void gos_app_deinit(void)
{
    settings->reset = true;
    gos_nvm_save();
}
