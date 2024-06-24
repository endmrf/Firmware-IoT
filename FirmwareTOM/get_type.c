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

#define READ_POST_DATA_TIMEOUT 1000

gos_result_t get_type_request(const gos_hs_request_t *request, const char *arg)
{
    gos_result_t result;
    char json_str[256];
    uint16_t bytes_read;

    uint8_t body_data[256];
	gos_buffer_t post_buffer = {
		.size = sizeof(body_data)-1,
		.data = body_data,
	};
    gos_json_parse_context_t *context;
    char _mac[40];

    GOS_LOG("get_type");

    if (gos_hs_request_get_method(request) != GOS_HTTP_METHOD_GET)
    {
        return gos_hs_return_status_code(request, 405, ERROR_405);
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
        gos_hs_return_status_code(request, 400, ERROR_400);
        goto exit;
    }

    if (GOS_FAILED(result, gos_hs_write_reply_header(request, "application/json", -1, GOS_HS_RESPONSE_FLAG_NONE)))
	{
		goto exit;
	}

	else if (GOS_FAILED(result, gos_hs_write_chunked_data(request, "{", sizeof("{")-1, false)))
	{
		goto exit;
	}

    char buffer[64];
	gos_network_get_mac(GOS_INTERFACE_WLAN, _mac);
	int len = snprintf(buffer, sizeof(buffer), "\"mac\":\"%s\",", _mac);
	len += snprintf(buffer + len, sizeof(buffer)-len, "\"type\":\"%s\"", DEVICE_TYPE);

    if (GOS_FAILED(result, gos_hs_write_chunked_data(request, buffer, len, false)))
	{
		goto exit;
	}

	if (GOS_FAILED(result, gos_hs_write_chunked_data(request, "}", sizeof("}")-1, true)))
	{
		goto exit;
	}

    exit:
    gos_json_parse_context_deinit(context);

    return result;
}
