/*
 * EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND
 * CONDITIONS OF THE CONTROLLING LICENSE AGREEMENT FOUND AT LICENSE.md
 * IN THIS SDK. IF YOU DO NOT AGREE TO THE LICENSE TERMS AND CONDITIONS,
 * PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.
 * (c) Copyright 2018, Silicon Laboratories Inc.  All rights reserved.
 */


#include "gos.h"
#include "mqtt_demo_common.h"
#include "connection.h"

// Commands created by this demo
GOS_CMD_CREATE_COMMAND(mqtt, connect,      "mqtt_connect",      "mq_cn", 0, 0, true);
GOS_CMD_CREATE_COMMAND(mqtt, disconnect,   "mqtt_disconnect",   "mq_dc", 0, 0, true);
GOS_CMD_CREATE_COMMAND(mqtt, publish,      "mqtt_publish",      "mq_pb", 2, 4, true);
GOS_CMD_CREATE_COMMAND(mqtt, subscribe,    "mqtt_subscribe",    "mq_sb", 1, 2, true);
GOS_CMD_CREATE_COMMAND(mqtt, unsubscribe,  "mqtt_unsubscribe",  "mq_us", 1, 1, true);
GOS_CMD_CREATE_COMMAND(mqtt, is_connected, "mqtt_is_connected", "mq_ic", 0, 0, true);



/**
 * Connect to the broker using the configuration from the CLI
 */
GOS_DEFINE_COMMAND(mqtt, connect)
{
    gos_result_t result;
    gos_mqtt_will_t* will;

    // Check if a will has been defined
    if ( strlen( mqtt_will->topic ) > 0 && strlen( settings->will_message ) > 0 )
    {
        will = mqtt_will;
    }
    else
    {
        will = NULL;
    }

    // Connect to the broker
    result = gos_mqtt_connect( mqtt_configuration, will, &mqtt_handle );
    if ( result != GOS_SUCCESS )
    {
        gos_cmd_print_str_response("Failed to connect to MQTT broker");
        return GOS_CMD_FAILED;
    }

    // Setup a default message handler so we can receive messages
    // That are published to topics we are subscribed to.
    // Note: we can also set topic specific handlers with
    //       gos_mqtt_set_message_handler
    result = gos_mqtt_set_default_message_handler( mqtt_handle, (gos_mqtt_publish_handler_t) publish_handler );
    if ( result != GOS_SUCCESS )
    {
        gos_cmd_print_str_response("Failed to set default message handler");
        return GOS_CMD_FAILED;
    }

    return GOS_CMD_EXECUTE_AOK;
}

/**
 * Disconnect from the broker
 */
GOS_DEFINE_COMMAND(mqtt, disconnect)
{
    gos_result_t result;

    result = gos_mqtt_disconnect( mqtt_handle );
    if ( result != GOS_SUCCESS )
    {
        return GOS_CMD_FAILED;
    }
    return GOS_CMD_EXECUTE_AOK;
}

/**
 * Publish to a topic
 * @param[in] topic Topic to publish to
 * @param[in] message Message to publish
 * @param[in] qos_level (optional) the MQTT quality of service level (0, 1, 2)
 * @param[in] retained (optional) If this message should be retained by the broker
 */
GOS_DEFINE_COMMAND(mqtt, publish)
{
    gos_result_t result;
    gos_mqtt_qos_level_t qos = MQTT_QOS_0;
    bool retained = false;

    if ( argc == 3 )
    {
        GOS_CMD_PARSE_INT_ARG_WITH_VAR( gos_mqtt_qos_level_t, qos, argv[2], 0, 2 );
    }
    else if ( argc == 4 )
    {
        GOS_CMD_PARSE_INT_ARG_WITH_VAR( bool, retained, argv[3], 0, 1 );
    }

    // Publish message with specified options
    result = gos_mqtt_publish( mqtt_handle, argv[0], qos, retained, argv[1], strlen( argv[1] ) );
    if ( result != GOS_SUCCESS )
    {
        return GOS_CMD_FAILED;
    }

    return GOS_CMD_EXECUTE_AOK;
}

/**
 * Subscribe to a topic
 * @param[in] topic Topic to subscribe to
 * @param[in] qos_level (optional) the MQTT quality of service level (0, 1, 2)
 */
GOS_DEFINE_COMMAND(mqtt, subscribe)
{
    // Default to QoS0
    gos_mqtt_qos_level_t qos = MQTT_QOS_0;

    if (argc == 2)
    {
        GOS_CMD_PARSE_INT_ARG_WITH_VAR(gos_mqtt_qos_level_t, qos, argv[1], 0, 2);
    }

    gos_result_t result;
    result = gos_mqtt_subscribe(mqtt_handle, argv[0], qos);

    if (result != GOS_SUCCESS)
    {
        return GOS_CMD_FAILED;
    }

    return GOS_CMD_EXECUTE_AOK;
}

/**
 * Unsubscribe from a topic
 * @param[in] topic Topic to unsubscribe from
 */
GOS_DEFINE_COMMAND(mqtt, unsubscribe)
{
    gos_result_t result;
    result = gos_mqtt_unsubscribe(mqtt_handle, argv[0]);

    if (result != GOS_SUCCESS)
    {
        return GOS_CMD_FAILED;
    }

    return GOS_CMD_EXECUTE_AOK;
}

/**
 * Check if currently connected to the broker
 */
GOS_DEFINE_COMMAND(mqtt, is_connected)
{
    if (gos_mqtt_is_currently_connected(mqtt_handle))
    {
        gos_cmd_print_str_response("Currently connected to broker");
    }
    else
    {
        gos_cmd_print_str_response("Not connected to broker");
    }
    return GOS_CMD_EXECUTE_AOK;
}
