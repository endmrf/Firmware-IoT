#include "commands.h"
bool toggle = false;
uint8_t countTime = 0;

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
#ifdef WTOM01
	gos_gpio_set(PLATFORM_LED1, status);
#endif
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
	gos_mqtt_disconnect(mqtt_handle);
	gos_network_down(GOS_INTERFACE_WLAN);
	set_ssid_name();
	load_pages();
	example_app_util_network_up(GOS_INTERFACE_SOFTAP, true, NULL);
	gos_event_register_timed(check_network, NULL, 180000, GOS_EVENT_FLAG_REQUIRE_SOFTAP);
}

void datetime_update (char* message_buffer)
{
	char *year, *month, *day, *hour, *minute, *second;
	char *p;
	gos_iso8601_t clock;
	gos_utc_ms_t rtc_ms;
	year = malloc(4);
	month = malloc(2);
	day = malloc(2);
	hour = malloc(2);
	minute = malloc(2);
	second = malloc(2);
	if ((p=strstr(message_buffer,":")) != NULL)
	{
		strncpy(year, (p+2), 4);
		strncpy(month, (p+7), 2);
		strncpy(day, (p+10), 2);
		strncpy(hour, (p+13), 2);
		strncpy(minute, (p+16), 2);
		strncpy(second, (p+19), 2);
		clock.year = atoi(year);
		clock.number_of_leap_years = 12;
		clock.month = atoi(month);
		clock.day = atoi(day);
		clock.hour = atoi(hour);
		clock.minute = atoi(minute);
		clock.second = atoi(second);
		clock.sub_second = 0;
		clock.timezone = 10800;
		clock.is_a_leap_year = false;
		gos_time_iso8601_to_utc_ms(&clock, &rtc_ms);
		gos_time_set_current_utc_ms(rtc_ms);
	}
	free(year);
	free(month);
	free(day);
	free(hour);
	free(minute);
	free(second);
}

void blink_event_handler()
{
    gos_gpio_set(PLATFORM_LED1, toggle);

    toggle = !toggle;
}

void counter_time()
{
	if(gos_button_is_active(PLATFORM_TOGGLE_OUTLET))
		countTime++;
	else if(countTime>=RESET_TIME/COUNT_PERIOD_MS)
	{
		gos_event_unregister(counter_time, NULL);
		reset_factory();
		countTime=0;
	}
	else
	{
		gos_event_unregister(counter_time, NULL);
		parametrization_mode();
		countTime=0;
	}
}
