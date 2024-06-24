#include "commands.h"


uint16_t level_dim[6] = {100, 80, 60, 40, 20, 0};
uint32_t level_intensity[6] = {1197, 2778, 3633, 4701, 5874, 8300};
//uint16_t level_intensity[7] = {8300, 5874, 4975, 4167, 3559, 2760, 2000};
bool timer_flag=false, pulse_1ms_1 = false, pulse_1ms_3 = false;
uint64_t pressed_time;

void receive_onoff_command (char* message_buffer)
{
	gos_result_t result; char brightness[100];
//	char* topic_buffer = PUBLISH_ONOFF_TOPIC;

	//if received command by app
	if(message_buffer != NULL)
	{
		if(strstr(message_buffer,"OFF"))
		{
			dimming = 5;
			up_down = false;
			settings->status = false;
		}
		else if(strstr(message_buffer, "ON"))
		{
			if (!GOS_FAILED(result, GOS_NVM_GET(MQTT, DEMO, settings)))
				dimming = settings->bright;

			if(dimming == 5)
			{
				dimming = 4;
				settings->bright = dimming;
			}

			settings->status = true;
		}
	}

	publish_status(settings->status);
	sprintf(brightness, "%i", level_dim[dimming]);
	publish_bright(brightness);
	//execute command
	//turn_on_off_device(settings->status);

	//save state on memory
	//gos_nvm_save();

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

void change_dimming (uint32_t button)
{
	char brightness[100];

	if(button == TOGGLE_BUTTON_2)
	{
		if(dimming > 0)
			dimming -= 1;
	}
	else if (button == TOGGLE_BUTTON_1)
	{
		if(dimming < 5)
			dimming += 1;
	}
	else if (button == PULSE_BUTTON)
	{
		GOS_LOG("Pulsador");
		pressed_time = 0;
		while(1){

			gos_time_set_current_utc_ms(0);
			while(!gos_button_is_active(PLATFORM_BUTTON_PULSE)){
				gos_time_get_current_utc_ms(&pressed_time, false);
				if(pressed_time > PULSE_TIME)
					break;
			}
			if(pressed_time > PULSE_TIME)
				break;

			if(up_down && (dimming > 0))
				dimming -= 1;
			else if(!up_down && (dimming < 5))
				dimming += 1;

			gos_rtos_delay_milliseconds( 1000 );
		}


	}
	settings->bright = dimming;
	//gos_nvm_save();

	if(dimming == 0 && button != PULSE_BUTTON)
		up_down = false;

	if(dimming == 5){
		publish_status(false);
		settings->status = false;
		if(button != PULSE_BUTTON)
			up_down = true;
	}
	else{
		publish_status(true);
		settings->status = true;
	}

	sprintf(brightness, "%i", level_dim[dimming]);
	publish_bright(brightness);

	//gos_gpio_irq_enable(PLATFORM_ZEROCROSS, GOS_GPIO_TRIGGER_RISING_EDGE, generate_trigger, NULL);
}

void brightSet (char* message_buffer)
{
	char *address_str, numbers[] = "0123456789", brightness[5]="", *address_number="", mensagem[100];

	sprintf(mensagem, "%s", message_buffer);

	address_str = strstr(mensagem, "brightSet");

	address_number = strpbrk(address_str, numbers);

//	address_str = strpbrk(message_buffer, numbers);
	while(address_number != NULL)
	{
		strcat(brightness, address_number);
		address_number = strpbrk(address_number+1, numbers);
	}

	dimming = (120 - atoi(brightness))/20 - 1; // fun��o para setar o n�vel de intensidade do dimmer
	/*if(atoi(brightness) == 80 || atoi(brightness) == 100)
		dimming -= 1;*/

	settings->bright = dimming;
	//gos_nvm_save();

	if(dimming == 0)
		up_down = false;

	if(dimming == 5){
		publish_status(false);
		settings->status = false;
		up_down = true;
	}
	else{
		publish_status(true);
		settings->status = true;
	}

	publish_bright(brightness);
}

void param_facReset ()
{

	uint64_t pressed_time = 0;
	uint8_t cont_param = 0;
	gos_time_set_current_utc_ms(0);
	while(gos_button_is_active(PLATFORM_TOGGLE1) || gos_button_is_active(PLATFORM_TOGGLE2)){
		gos_rtos_delay_milliseconds(1);
		gos_time_get_current_utc_ms(&pressed_time, false);
		if(pressed_time>=PARAM_TIME){
			gos_event_unregister(check_connection_mqtt, NULL);
			if(dimming == 5 && cont_param == 0)
			{
				dimming = 0;
				gos_rtos_delay_milliseconds(200);
				dimming = 5;
				cont_param++;
			}
			else if(dimming == 0 && cont_param == 0)
			{
				dimming = 5;
				gos_rtos_delay_milliseconds(200);
				dimming = settings->bright;
				cont_param++;
			}
		}
	}
	gos_time_get_current_utc_ms(&pressed_time, false);

	GOS_LOG("Pressed_time: %d", pressed_time);
//	gos_gpio_irq_disable( PLATFORM_ZEROCROSS );
	if(pressed_time >= PARAM_TIME)
		reset_factory();
	else if(pressed_time>=PARAM_TIME && pressed_time<RESET_TIME)
		parametrization_mode();

//	gos_gpio_irq_enable(PLATFORM_ZEROCROSS, GOS_GPIO_TRIGGER_RISING_EDGE, generate_trigger, NULL);
}

void stop_trigger()
{
	gos_gpio_set(PLATFORM_RELAY, false);
	gos_rtos_timer_stop( &timer );
}

void drive_triac()
{
	if(dimming != 5)
	{
		uint32_t timer_cycle_count = (CAP_DELAY + level_intensity[dimming]) * ((CMU_ClockFreqGet( cmuClock_HFPER )/1000000)/TIMER_PRESCALER);
		if(!timer_flag)
		{
		  TIMER_CounterSet(TIMERn, timer_cycle_count);
		  TIMER_IntClear(TIMERn, TIMER_IEN_UF);
		  TIMER_IntEnable(TIMERn, TIMER_IEN_UF);
		  TIMER_Enable(TIMERn, true);
		  timer_flag = true;
		}
		else
		{
		  TIMER_CounterSet(TIMER3n, timer_cycle_count);
		  TIMER_IntClear(TIMER3n, TIMER_IEN_UF);
		  TIMER_IntEnable(TIMER3n, TIMER_IEN_UF);
		  TIMER_Enable(TIMER3n, true);
		  timer_flag = false;
		}
	}
}

void initTimer(void)
{
  // Enable clock for TIMERn module
  CMU_ClockEnable(TIMERn_CMU_CLOCK, true);
  CMU_ClockEnable(TIMER3n_CMU_CLOCK, true);
  // Initialize the timer...this example uses one-shot countdown mode


  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
  timerInit.enable = false;
  timerInit.oneShot = true; // Generate only one pulse
  timerInit.mode = timerModeDown;    //count down
  timerInit.prescale = timerPrescale1;
  TIMER_Init(TIMERn, &timerInit);
  TIMER_Init(TIMER3n, &timerInit);
}

void TIMERn_IRQ_HANDLER(void)
{
  uint32_t flags = TIMER_IntGet(TIMERn);
  TIMER_IntClear(TIMERn, flags);

  if(flags & TIMER_IF_UF){
    TIMER_Enable(TIMERn, false);

    if(!pulse_1ms_1)
    {
    	gos_gpio_set(PLATFORM_RELAY, true);
		TIMER_CounterSet(TIMERn, timer_pulse);
		TIMER_IntClear(TIMERn, TIMER_IEN_UF);
		TIMER_IntEnable(TIMERn, TIMER_IEN_UF);
		pulse_1ms_1 = true;
		TIMER_Enable(TIMERn, true);
    }
    else
	{
		gos_gpio_set(PLATFORM_RELAY, false);
		pulse_1ms_1 = false;
	}
  }

}

void TIMER3n_IRQ_HANDLER(void)
{
  uint32_t flags = TIMER_IntGet(TIMER3n);
  TIMER_IntClear(TIMER3n, flags);

  if(flags & TIMER_IF_UF){
    TIMER_Enable(TIMER3n, false);
    if(!pulse_1ms_3)
    {
    	gos_gpio_set(PLATFORM_RELAY, true);
		TIMER_CounterSet(TIMER3n, timer_pulse);
		TIMER_IntClear(TIMER3n, TIMER_IEN_UF);
		TIMER_IntEnable(TIMER3n, TIMER_IEN_UF);
		pulse_1ms_3 = true;
		TIMER_Enable(TIMER3n, true);
    }
    else
    {
    	gos_gpio_set(PLATFORM_RELAY, false);
    	pulse_1ms_3 = false;
    }

  }

}
