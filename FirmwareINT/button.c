/*
 * button.c
 *
 *  Created on: 12 de ago de 2019
 *      Author: Joao Paulo
 */

#include "button.h"
#include "commands.h"

static void button_pressed_event_handler(void *arg);
static void button_clicked_event_handler(void *arg);
//static void button_toggled_event_handler(void *arg);

uint64_t pressed_time;
bool toggle = false;

gos_button_config_t config =
{
	.active_level         = PLATFORM_BUTTON_ACTIVE_STATE,
	.debounce             = BUTTON_DEBOUNCE_TIME_MS,
	.click_time           = BUTTON_CLICK_TIME_MS,
	.press_time           = BUTTON_PRESS_TIME_MS,
	.event_handler.press  = button_pressed_event_handler,
	.event_handler.click  = button_clicked_event_handler,
//	.event_handler.toggle = button_toggled_event_handler,
	.execution_context    = GOS_BUTTON_CONTEXT_EVENT_THREAD
};

static void button_pressed_event_handler(void *arg)
{
    uint32_t button_number = (uint32_t)arg;

    if(button_number != THREEWAY_BUTTON)
    {

		GOS_LOG("Button%d: pressed", button_number);
		gos_rtos_thread_delete( &toggle_thread );
		gos_event_unregister(check_connection_mqtt, NULL);
		receive_onoff_command (NULL);
		receive_onoff_command (NULL);
		gos_time_set_current_utc_ms(0);
		while(gos_button_is_active(PLATFORM_TOGGLE1) || gos_button_is_active(PLATFORM_TOGGLE2)){
			gos_rtos_delay_milliseconds(1);
		}
		gos_time_get_current_utc_ms(&pressed_time, false);

		GOS_LOG("Pressed_time: %d", pressed_time);

		if(pressed_time >= RESET_TIME){
			reset_factory();
		}
		else{
			parametrization_mode();
		}
    }

}

static void button_clicked_event_handler(void *arg)
{
    uint32_t button_number = (uint32_t)arg;

    GOS_LOG("Button%d: click", button_number);
    gos_rtos_thread_delete( &toggle_thread );
    if(button_number != THREEWAY_BUTTON)
    {
    	receive_onoff_command(NULL);
    }

    gos_rtos_thread_create( &toggle_thread, &config_thread);

}

void button_init(){
		gos_button_init(PLATFORM_TOGGLE1, &config, (void*)TOGGLE_BUTTON_1);
		#ifndef WTOM01
			gos_button_init(PLATFORM_TOGGLE2, &config, (void*)TOGGLE_BUTTON_2);
//			config.debounce = 10;
			gos_button_init(PLATFORM_BUTTON_PULSE, &config, (void*)THREEWAY_BUTTON);
			gos_time_set_current_utc_ms(0);
			while(!gos_button_is_active(PLATFORM_BUTTON_PULSE)){ //Read 3WAY Status
				gos_time_get_current_utc_ms(&pressed_time, false);
				if(pressed_time>8){
					toggle = true;
					break;
				}
			}
		#else
			gos_gpio_init(PLATFORM_LED1, GOS_GPIO_OUTPUT_PUSH_PULL, false);
		#endif
		gos_gpio_init(PLATFORM_RELAY, GOS_GPIO_OUTPUT_PUSH_PULL, false);
}


void toggle_handler()
{
	while(1)
	{
		gos_rtos_delay_milliseconds(10);
		if(gos_button_is_active(PLATFORM_BUTTON_PULSE) && toggle)
		{
			receive_onoff_command(NULL);
			toggle = !toggle;
		}
		else if (!toggle)
		{
			gos_time_set_current_utc_ms(0);
			while(!gos_button_is_active(PLATFORM_BUTTON_PULSE)){
				gos_time_get_current_utc_ms(&pressed_time, false);
				if(pressed_time>10){
					receive_onoff_command(NULL);
					toggle = !toggle;
					break;
				}
			}
		}
	}
}
