/*
 * EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND
 * CONDITIONS OF THE CONTROLLING LICENSE AGREEMENT FOUND AT LICENSE.md
 * IN THIS SDK. IF YOU DO NOT AGREE TO THE LICENSE TERMS AND CONDITIONS,
 * PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.
 * (c) Copyright 2018, Silicon Laboratories Inc.  All rights reserved.
 */

#include "gos.h"
#include "common.h"
#include "mqtt.h"
#include "mqtt_demo_common.h"
#include "nvm_settings.h"
#include "connection.h"

device_configuration_t device_configuration;
network_credentials_t network_credentials;

gos_result_t json_parser_request(const gos_hs_request_t *request, const char *arg)
{
    gos_result_t result;
    char *vazio = "";
    uint16_t bytes_read;
    char* rede, *password;
    gos_json_parse_context_t *context;

    uint8_t body_data[256];
    gos_buffer_t post_buffer = {
    		.size = sizeof(body_data)-1,
            .data = body_data,
    };

    char _mac[40];
    const gos_json_tok_t *ssid_tok, *password_tok, *orgID_tok, *deviceID_tok, *passAuth_tok, *brokerPort_tok;

    settings->configuration.broker_port = 0;
	sprintf(settings->configuration.password, "%s", vazio);
	sprintf(settings->configuration.client_id, "%s", vazio);
	mqtt_configuration        = &(settings->configuration);

    gos_gpio_irq_disable( PLATFORM_ZEROCROSS );

    if (gos_hs_request_get_method(request) != GOS_HTTP_METHOD_POST)
    {
        return gos_hs_return_status_code(request, 405, "<h1>Invalid Request Method. Must be POST</h1>");
    }
    else if (GOS_FAILED(result, gos_hs_read_post_data(request, &post_buffer, READ_POST_DATA_TIMEOUT)))
	{
		return gos_hs_return_status_code(request, 500, ERROR_500);
	}

	const gos_json_parse_config_t cfg =
	{
			.buffer = post_buffer.data,
			.buffer_len = (uint16_t)post_buffer.size
	};

    gos_json_parse_context_init(&context, &cfg);

    if (GOS_FAILED(result, gos_json_parse_chunked(context, NULL)))
    {
        gos_hs_return_status_code(request, 400, "<h1>Failed to parse JSON</h1>");
        goto exit;
    }

    ssid_tok      = gos_json_context_get_token(context, "ssid", NULL);
	password_tok  = gos_json_context_get_token(context, "password", NULL);
	orgID_tok  = gos_json_context_get_token(context, "orgID", NULL);
	deviceID_tok  = gos_json_context_get_token(context, "deviceID", NULL);
	passAuth_tok  = gos_json_context_get_token(context, "token", NULL);
	brokerPort_tok = gos_json_context_get_token(context, "port", NULL);

	if ((ssid_tok    == NULL) || (ssid_tok->next    == NULL)  ||
		(password_tok  == NULL) || (password_tok->next  == NULL) || (orgID_tok  == NULL)
		|| (orgID_tok->next  == NULL) || (deviceID_tok  == NULL) || (deviceID_tok->next  == NULL) ||
		(passAuth_tok  == NULL) || (passAuth_tok->next  == NULL) || (brokerPort_tok == NULL) || (brokerPort_tok->next == NULL))
	{
		gos_hs_return_status_code(request, 400, "<h1>JSON not formatted correctly</h1>" \
												 "Must be of the form:<br />" \
												 "{\"ssid\":<ssid_str>,\"password\":<password_str>,\"orgID\":<orgID_str>,\"deviceID\":<deviceID_str>,\"passAuth\":<passAuth_str>,\"port\":<port_str>\"}");
		goto exit;
	}

    rede = ssid_tok->next->data.str;
    password = password_tok->next->data.str;
    //led_value = light_tok->next->data.boolean;
    GOS_LOG("SSID: %s", ssid_tok->next->data.str);
    GOS_LOG("PASSWORD: %s", password_tok->next->data.str);
    GOS_LOG("OrgID: %s", orgID_tok->next->data.str);
    GOS_LOG("DeviceID: %s", deviceID_tok->next->data.str);
    GOS_LOG("TOKEN: %s", passAuth_tok->next->data.str);
    GOS_LOG("PORT: %s", brokerPort_tok->next->data.str);

	if (GOS_FAILED(result, gos_hs_write_reply_header(request, "application/json", -1, GOS_HS_RESPONSE_FLAG_NONE)))
	{
		goto exit;
	}


	else if (GOS_FAILED(result, gos_hs_write_chunked_data(request, "{", sizeof("{")-1, false)))
	{
		goto exit;
	}

	gos_system_get_version_details(&version_firmware, version_str_buffer);
	char buffer[128], serial_number[7];
	gos_network_get_mac(GOS_INTERFACE_WLAN, _mac);
	int len = snprintf(buffer, sizeof(buffer), "\"mac\":\"%s\",", _mac);
	len += snprintf(buffer + len, sizeof(buffer)-len, "\"type\":\"%s\",", DEVICE_TYPE);
	//len += snprintf(buffer + len, sizeof(buffer)-len, "\"ip\":\"%s\"", DEVICE_TYPE);
	len += snprintf(buffer + len, sizeof(buffer)-len, "\"hw_version\":\"%s\",", HW_VERSION);
	len += snprintf(buffer + len, sizeof(buffer)-len, "\"fw_version\":\"%s\",", version_firmware.bundle.version_str);
	removeChar(_mac, ":");
	snprintf(serial_number, sizeof(serial_number), "%s", &_mac[6]);
	len += snprintf(buffer + len, sizeof(buffer)-len, "\"serial_number\":\"%s\"", serial_number);
	GOS_LOG("%s\n", buffer);

	if (GOS_FAILED(result, gos_hs_write_chunked_data(request, buffer, len, false)))
	{
		goto exit;
	}

	if (GOS_FAILED(result, gos_hs_write_chunked_data(request, "}", sizeof("}")-1, true)))
	{
		goto exit;
	}

	const gos_ssid_t rede_scan;
	uint8_t *rede_len;
	strncpy(&rede_scan.value, rede, strlen(rede));
	rede_len = &rede_scan.length;
	*rede_len=strlen(rede);

	gos_scan_result_t *result_list;
	result = gos_network_scan(&result_list, 0, &rede_scan);
	if(result == GOS_SUCCESS){
		result = gos_network_verify((const gos_scan_result_t *)result_list, (const uint8_t *)password, strlen(password), 5000);
		if(result == GOS_SUCCESS){
			GOS_LOG("Rede OK");

			strncpy(device_configuration.deviceID, deviceID_tok->next->data.str, strlen(deviceID_tok->next->data.str));
			GOS_LOG("param: %s", device_configuration.deviceID);
			strncpy(device_configuration.orgID , orgID_tok->next->data.str, strlen(orgID_tok->next->data.str));
			GOS_LOG("param: %s", device_configuration.orgID);
			strncpy(device_configuration.password_device, passAuth_tok->next->data.str, strlen(passAuth_tok->next->data.str));
			GOS_LOG("param: %s", device_configuration.password_device);
			strncpy(device_configuration.brokerPort, brokerPort_tok->next->data.str, strlen(brokerPort_tok->next->data.str));
			GOS_LOG("param: %s", device_configuration.brokerPort);
			strncpy(network_credentials.ssid, rede, strlen(rede));
			GOS_LOG("param: %s", network_credentials.ssid);
			strncpy(network_credentials.password, password, strlen(password));
			GOS_LOG("param: %s", network_credentials.password);

			gos_network_down(GOS_INTERFACE_SOFTAP);
			uint8_t attempts = 0;
			gos_network_set_credentials(GOS_INTERFACE_WLAN, network_credentials.ssid, network_credentials.password, false);

			while(attempts <= 2)
			{
			   if(GOS_SUCCESS == gos_network_up(GOS_INTERFACE_WLAN, true))
			   {
				   gos_event_issue((void *)connect_wlan_handler, NULL, GOS_EVENT_FLAG_NONE);
				//   system_up=true;
				   break;
			   }
			   gos_network_restart(GOS_INTERFACE_WLAN);
			   attempts++;
			}

//			if(!gos_network_is_up(GOS_INTERFACE_WLAN))
//			   gos_system_reboot();


		}
		else{
			GOS_LOG("[network_verify]resultado da falha: %d", result);

		}
	}

	gos_nvm_save();

	exit:

	if(!gos_network_is_up(GOS_INTERFACE_WLAN))
	   gos_system_reboot();

	gos_network_scan_destroy_results();
	gos_json_parse_context_deinit(context);

	return result;
}
