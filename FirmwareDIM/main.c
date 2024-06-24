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
gos_timer_t timer;
uint32_t timer_pulse;
/*uint16_t dimming_ant;*/
gos_thread_t thread_pulse;

gos_thread_config_t config_pulse =
{
	.function = check_pulse,
	.stack_size = 4096,
	.stack_buffer = 0,
	.arg = 0,
	.priority = GOS_THREAD_PRIORITY_HIGHER
};

#define MESSAGE_BUFFER_LENGTH  256


/*************************************************************************************************/
void gos_app_init(void)
{
	gos_result_t result;

	timer_pulse = DELAY_TIME_PULSE * ((CMU_ClockFreqGet( cmuClock_HFPER )/1000000)/TIMER_PRESCALER);

	initTimer();

	NVIC_EnableIRQ(TIMERn_IRQ);
	gos_system_set_irq_callback(TIMERn_IRQ, TIMERn_IRQ_HANDLER);

	NVIC_EnableIRQ(TIMER3n_IRQ);
	gos_system_set_irq_callback(TIMER3n_IRQ, TIMER3n_IRQ_HANDLER);

	button_init();
//	gos_rtos_thread_create(&thread_save, &config_save);

	if (!GOS_FAILED(result, GOS_NVM_GET(MQTT, DEMO, settings)))
	{
		/*if(settings->status)
			dimming = settings->bright;
		else*/
			dimming = 0;
	}

	gos_rtos_thread_create(&thread_pulse, &config_pulse);

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
