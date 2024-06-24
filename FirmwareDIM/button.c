
#include "button.h"

static void button_pressed_event_handler(void *arg);
static void button_clicked_event_handler(void *arg);

uint16_t dimming, count_up = 0;
bool up_down = true;

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

gos_button_config_t config2 =
{
	.active_level         = PLATFORM_BUTTON_ACTIVE_STATE,
	.debounce             = PULSE_DEBOUNCE_TIME_MS,
//	.click_time           = PULSE_CLICK_TIME_MS,
//	.press_time           = PULSE_PRESS_TIME_MS,
//	.event_handler.press  = button_pressed_event_handler,
//	.event_handler.click  = button_clicked_event_handler,
//	.event_handler.toggle = button_toggled_event_handler,
	.execution_context    = GOS_BUTTON_CONTEXT_EVENT_THREAD
};

static void button_pressed_event_handler(void *arg)
{
	uint32_t button_number = (uint32_t)arg; char brightness[100];
	GOS_LOG("Button%d: pressed", button_number);

	if(button_number == BUTTON_MINUS)
	{
		dimming = 5;
		up_down = true;
		param_facReset();
		settings->status = false;
//		settings->bright = dimming;
		publish_status(false);
	}
	else if(button_number == BUTTON_PLUS)
	{
		dimming = 0;
		up_down = false;
		param_facReset();
		settings->status = true;
		settings->bright = dimming;
		publish_status(true);
		sprintf(brightness, "%i", level_dim[dimming]);
		publish_bright(brightness);
	}
	else if(button_number == PULSE_BUTTON)
	{
		change_dimming(button_number);
		up_down = !up_down;
		GOS_LOG("up_down: %i", up_down);
	}

	/*if(button_number != PULSE_BUTTON)
		//gos_nvm_save();*/
	/*gos_event_unregister(button_clicked_event_handler, NULL);
	gos_event_unregister(button_pressed_event_handler, NULL);*/
}

static void button_clicked_event_handler(void *arg)
{
	uint32_t button_number = (uint32_t)arg; gos_result_t result;
	GOS_LOG("Button%d: clicked", button_number); char brightness[100];

//	gos_event_unregister(button_toggled_event_handler, NULL);

	if(!settings->status && (button_number == TOGGLE_BUTTON_2 || button_number == PULSE_BUTTON))
	{

		if (!GOS_FAILED(result, GOS_NVM_GET(MQTT, DEMO, settings)))
		{
			dimming = settings->bright;
			sprintf(brightness, "%i", level_dim[dimming]);
			publish_bright(brightness);
		}
		if(dimming == 5 && button_number != PULSE_BUTTON)
			change_dimming(button_number);
		else if(dimming == 5 && button_number == PULSE_BUTTON)
		{
			dimming = 0;
			up_down = false;
			settings->bright = dimming;
			sprintf(brightness, "%i", level_dim[dimming]);
			publish_bright(brightness);
		}
		else if(dimming == 0 && button_number == PULSE_BUTTON)
			up_down = false;

		settings->status = true;
	}
	else if (settings->status && button_number != PULSE_BUTTON)
		change_dimming(button_number);
	else if (settings->status && button_number == PULSE_BUTTON)
	{
			settings->bright = dimming;
			dimming = 5;
			up_down = true;
			settings->status = false;
	}


	publish_status(settings->status);
	//gos_nvm_save();
}

void button_init(){
	gos_button_init(PLATFORM_TOGGLE1, &config, (void*)TOGGLE_BUTTON_1);
	gos_button_init(PLATFORM_TOGGLE2, &config, (void*)TOGGLE_BUTTON_2);
	gos_button_init(PLATFORM_BUTTON_PULSE, &config2, (void*)PULSE_BUTTON);

	gos_gpio_init(PLATFORM_RELAY, GOS_GPIO_OUTPUT_PUSH_PULL, false);
	gos_gpio_init(PLATFORM_ZEROCROSS, GOS_GPIO_INPUT_HIGH_IMPEDANCE, false);

	gos_gpio_irq_enable(PLATFORM_ZEROCROSS, GOS_GPIO_TRIGGER_FALLING_EDGE, drive_triac, NULL);
}

void check_pulse ()
{
	while(1)
	{
		gos_rtos_delay_milliseconds(1);
		if(gos_button_is_active(PLATFORM_BUTTON_PULSE))
		{
			gos_rtos_delay_milliseconds(500);
			gos_time_set_current_utc_ms(0);
			while(!gos_button_is_active(PLATFORM_BUTTON_PULSE)){
				gos_time_get_current_utc_ms(&pressed_time, false);
				if(pressed_time > 50){
					GOS_LOG("Clicked");
					button_clicked_event_handler((void*)PULSE_BUTTON);
					break;
				}
			}
			if(gos_button_is_active(PLATFORM_BUTTON_PULSE)){
				GOS_LOG("Pressed");
				change_dimming(PULSE_BUTTON);
				up_down = !up_down;
				GOS_LOG("up_down: %i", up_down);
			}
		}
	}
}
