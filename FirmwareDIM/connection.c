#include "connection.h"

//uint16_t level_dim[7] = {0, 20, 35, 50, 65, 80, 100};
gos_mqtt_configuration_t* mqtt_configuration;
gos_mqtt_will_t* mqtt_will;
gos_mqtt_handle_t mqtt_handle;
gos_version_details_t version_firmware;
char version_str_buffer[100];

void removeChar(char *src, char *character);

void publish_handler(gos_mqtt_handle_t handle, gos_mqtt_message_t* message)
{

	char     message_buffer[MESSAGE_BUFFER_LENGTH], topic_buffer[64], *address_str;
	uint32_t payload_length = MIN( MESSAGE_BUFFER_LENGTH - 1, message->payload_length );

	strncpy( message_buffer, (char *) message->payload, payload_length );
	message_buffer[payload_length] = '\0';
	strncpy( topic_buffer, message->topic, strlen(message->topic));
	topic_buffer[strlen(message->topic)] = '\0';

	address_str = strstr(topic_buffer, "cmd");
	strncpy(address_str, "evt", 3);

	if(strstr(topic_buffer, "firmware") != NULL){
		if(GOS_SUCCESS == gos_mqtt_publish(mqtt_handle, PUBLISH_UPDATEFIRMWARE_TOPIC, 1, true, PUBLISH_UPDATING_MESSAGE, strlen(PUBLISH_UPDATING_MESSAGE)))
			GOS_LOG("Updating");
		gos_dms_ota_update(NULL, ALLOW_MULTIPASS_UPDATES);
	}
	else if(strstr(topic_buffer, "brightSet") != NULL)
	{
		GOS_LOG("BrightSet event");
		brightSet(message_buffer);
	}
	else if(strstr(topic_buffer, "onOff") != NULL)
	{
		receive_onoff_command (message_buffer);
	}

	GOS_LOG("[MQTT] Received from topic: '%s', Message: '%s'", message->topic, message_buffer);

}

void setup_mqtt (const gos_json_tok_t *passAuth, const gos_json_tok_t *orgID, const gos_json_tok_t *deviceID, const gos_json_tok_t *brokerPort)
{
	gos_result_t result; char brightness[100];

	if (GOS_FAILED(result, GOS_NVM_GET(MQTT, DEMO, settings)))
	{
		GOS_LOG("Error retrieving configuration from NVM, error code: %d", result);
		return;
	}

		// Check the magic number has been set to avoid attempting to initialize using random values
	if( settings->magic != MQTT_SETTINGS_MAGIC )
	{
		GOS_LOG("NVM settings uninitialized");
		return;
	}

	if(settings->configuration.broker_port == 0)
	{
		settings->configuration.broker_port = atoi(brokerPort->next->data.str);
	}

	if(strnlen(settings->configuration.password, MAX_PASSWORD_LENGTH) == 0)
	{
		sprintf(settings->configuration.password, "%s", passAuth->next->data.str);
	}

	if(strnlen(settings->configuration.client_id, MAX_CLIENT_ID_LENGTH) == 0)
	{
		sprintf(settings->configuration.client_id, "d:%s:%s:%s", orgID->next->data.str, DEVICE_TYPE, deviceID->next->data.str);

	}

	mqtt_configuration        = &(settings->configuration);
	mqtt_will                 = &(settings->will);
	gos_nvm_save();

	if (GOS_FAILED(result, gos_load_app_settings("network_tls.ini")))
	{
		GOS_LOG("Failed to load network_tls, err:%d", result);
		return;
	}

	if ( GOS_FAILED( result, gos_mqtt_init( ) ) )
    {
	   GOS_LOG( "Failed to initialize MQTT: %d", result );
	   return;
    }

	//while(!gos_mqtt_is_currently_connected(mqtt_handle))
	//{
		if(GOS_SUCCESS == gos_mqtt_connect( mqtt_configuration, NULL, &mqtt_handle))
		{
			GOS_LOG("Connected broker successfully");

			result = gos_mqtt_set_default_message_handler( mqtt_handle, (gos_mqtt_publish_handler_t) publish_handler );
			if(result != GOS_SUCCESS)
			   GOS_LOG("Failed to set default message handler");

			if(GOS_SUCCESS == gos_mqtt_subscribe(mqtt_handle, ALL_TOPICS, 1))
			   GOS_LOG("Topics subscribed successfully");

			publish_status(settings->status);
			sprintf(brightness, "%i", level_dim[dimming]);
			publish_bright(brightness);
			publish_firmwareStatus();
		}
	//}

    gos_system_reset_faults();
    gos_event_register_periodic(check_connection_mqtt, NULL, 5000, GOS_EVENT_FLAG_NONE);

}

void check_connection_mqtt()
{
	char brightness[100];
	if(!gos_mqtt_is_currently_connected(mqtt_handle))
	{
		GOS_LOG("OK");
		if(GOS_SUCCESS == gos_mqtt_connect( mqtt_configuration, NULL, &mqtt_handle))
		{
			gos_mqtt_set_default_message_handler( mqtt_handle, (gos_mqtt_publish_handler_t) publish_handler );
			gos_mqtt_subscribe(mqtt_handle, ALL_TOPICS, 1); //subscribe on all topics
			publish_status(settings->status);
			sprintf(brightness, "%i", level_dim[dimming]);
			publish_bright(brightness);
		}
//		gos_mqtt_publish(mqtt_handle, PUBLISH_DATETIME_TOPIC, 1, true, "{}", strlen("{}")); //request date time
	}
}

void check_network()
{
	gos_network_down(GOS_INTERFACE_SOFTAP);

	if (settings->configuration.broker_port != 0)
		setup_mqtt(NULL, NULL, NULL, NULL);
}

void set_ssid_name()
{
	char ssid[20];
    char _mac[18];
    gos_network_get_mac(GOS_INTERFACE_WLAN, _mac);
    removeChar(_mac, ":");
    snprintf(ssid, sizeof(ssid), "WEG%s_%s", DEVICE_TYPE, &_mac[6]);

    GOS_LOG("ssid: %s", ssid);
    gos_settings_set_str("softap.ssid" , ssid);
}

void load_pages()
{
	GOS_HTTP_SERVER_REGISTER_DYNAMIC_PAGES();
}

void removeChar(char *src, char *character)
{
	char *p;
	while ((p=strstr(src,character)) != NULL)
	{
		memmove(p,p+strlen(character), strlen(p+strlen(character))+1);
	}
}

void publish_status(bool status){

	char* message_buffer;
	gos_result_t result;
	char* topic_buffer = PUBLISH_ONOFF_TOPIC;
	if (status == false)
	{
		message_buffer = PUBLISH_OFF_MESSAGE;
	}
	else
	{
		message_buffer = PUBLISH_ON_MESSAGE;
	}
	if(gos_mqtt_is_currently_connected(mqtt_handle))
	{
		//publish status
		GOS_LOG( "[MQTT] Message: %s Write status on Topic %s", message_buffer, topic_buffer);
		result = gos_mqtt_publish(mqtt_handle, topic_buffer, 1, true, message_buffer, strlen(message_buffer));
		if(result == GOS_SUCCESS)
			GOS_LOG("Successfully published message");
	}

}

void publish_bright(char* brightness)
{
	char message[100]; gos_result_t result;

	sprintf(message, "{\"d\":{\"brightSetStatus\":\"%i\"}}", atoi(brightness));
	GOS_LOG("Mensagem: %s", message);

	result = gos_mqtt_publish(mqtt_handle, PUBLISH_BRIGHTSET_TOPIC, 1, true, message, strlen(message));
	if(result == GOS_SUCCESS)
		GOS_LOG("Successfully published message");
}

void publish_firmwareStatus()
{
	char message[100];

	gos_system_get_version_details(&version_firmware, version_str_buffer);
	GOS_LOG("Version: %s", version_firmware.bundle.version_str);
	sprintf(message, "{\"d\":{\"firmwareUpdateStatus\": \"%s\"}}", version_firmware.bundle.version_str);
	if(GOS_SUCCESS == gos_mqtt_publish(mqtt_handle, PUBLISH_UPDATEFIRMWARE_TOPIC, 1, true, message, strlen(message)))
		GOS_LOG("Firmware publicado com sucesso");
}

void connect_wlan_handler(){
	//if(system_up){
	setup_mqtt(device_configuration.password_device, device_configuration.orgID, device_configuration.deviceID, device_configuration.brokerPort);
		//system_up=false;
	//}
}
