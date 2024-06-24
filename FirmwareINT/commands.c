#include "commands.h"

void receive_onoff_command (char* message_buffer)
{
	//if received command by app
	if(message_buffer != NULL)
	{
		if(strstr(message_buffer,"OFF"))
		{
			settings->status = false;
		}
		else if(strstr(message_buffer, "ON"))
		{
			settings->status = true;
		}
	}
	else // if received command by button
	{
		settings->status = !(settings->status);
	}

	//execute command
	turn_on_off_device(settings->status);

	//publish status
	publish_status(settings->status);

	//save state on memory
	gos_nvm_save();
}


void turn_on_off_device(bool status)
{
	gos_gpio_set(PLATFORM_RELAY, status);
}

void reset_factory()
{
	gos_network_set_credentials(GOS_INTERFACE_WLAN, "", "", true);
	gos_network_down(GOS_INTERFACE_WLAN);
	gos_nvm_factory_reset(GOS_NVM_RESET_FACTORY);
}

void parametrization_mode()
{
	//GOS_LOG("Parametrization");
	settings->reset = false;
	gos_rtos_thread_create( &toggle_thread, &config_thread);
	gos_mqtt_disconnect(mqtt_handle);
	gos_network_down(GOS_INTERFACE_WLAN);
	set_ssid_name();
	load_pages();
	example_app_util_network_up(GOS_INTERFACE_SOFTAP, true, NULL);
	gos_event_register_timed(check_network, NULL, 180000, GOS_EVENT_FLAG_REQUIRE_SOFTAP);
}
