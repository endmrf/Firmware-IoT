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
#ifndef WTOM01
static void button_toggled_event_handler(void *arg);
#endif

uint64_t pressed_time;

gos_button_config_t config =
{
	.active_level         = PLATFORM_BUTTON_ACTIVE_STATE,
	.debounce             = BUTTON_DEBOUNCE_TIME_MS,
	.click_time           = BUTTON_CLICK_TIME_MS,
	.press_time           = BUTTON_PRESS_TIME_MS,
	.event_handler.press  = button_pressed_event_handler,
	.event_handler.click  = button_clicked_event_handler,
	//.event_handler.toggle = button_toggled_event_handler,
	.execution_context    = GOS_BUTTON_CONTEXT_IRQ
};

static void button_pressed_event_handler(void *arg)
{
    uint32_t button_number = (uint32_t)arg;

	GOS_LOG("Button%d: pressed", button_number);
	gos_event_unregister(check_connection_mqtt, NULL);
	gos_event_register_periodic(blink_event_handler, NULL, BLINK_PERIOD_MS, GOS_EVENT_FLAG_NONE);
	gos_event_register_periodic(counter_time, NULL, COUNT_PERIOD_MS, GOS_EVENT_FLAG_NONE);

}

static void button_clicked_event_handler(void *arg)
{
    uint32_t button_number = (uint32_t)arg;

    GOS_LOG("Button%d: click", button_number);

    gos_event_issue((void *)receive_onoff_command, NULL, GOS_EVENT_FLAG_NONE);


}

void button_init(){
		gos_button_init(PLATFORM_TOGGLE_OUTLET, &config, (void*)TOGGLE_BUTTON_1);
		gos_gpio_init(PLATFORM_LED1, GOS_GPIO_OUTPUT_PUSH_PULL, false);
		gos_gpio_init(PLATFORM_RELAY, GOS_GPIO_OUTPUT_PUSH_PULL, false);
}

