/*
 * EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND
 * CONDITIONS OF THE CONTROLLING LICENSE AGREEMENT FOUND AT LICENSE.md
 * IN THIS SDK. IF YOU DO NOT AGREE TO THE LICENSE TERMS AND CONDITIONS,
 * PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.
 * (c) Copyright 2018, Silicon Laboratories Inc.  All rights reserved.
 */


#include "gos.h"
#include "mqtt_demo_common.h"

// Setters and getters for connection configuration
/**
 * \param mqtt.auto_reconnect           Automatically attempt to reconnect to the broker if the underlying network connection becomes disconnected.
 *                                      Note: if the clean session flag is set in the configuration then any pending messages and subscriptions will
 *                                            purposefully be forgotten by the broker on reconnect.
 * \param mqtt.broker_hostname          Hostname or IP address of the broker
 * \param mqtt.broker_port              Port to use for the broker connection (typically 1883, or 8883 for TLS)
 * \param mqtt.clean_session            When true the broker should delete any previous stored undelivered messages and subscription information when connecting
 * \param mqtt.client_id                A client ID for this connection (must be unique for the broker). Must be null terminated.
 * \param mqtt.keep_alive_interval      MQTT keep alive interval in seconds
 * \param mqtt.password                 Optional password for this connection. Must be null terminated.
 * \param mqtt.username                 Optional username for this connection. Must be null terminated.
 * \param mqtt.use_tls                  If the connection should use TLS instead of TCP (note: broker_port is not automatically updated)
 * \param mqtt.version                  MQTT protocol to use for this connection, "3.1" or "3.1.1" are supported
 * \param mqtt.will.message             MQTT will message to be sent. Must be null terminated.
 * \param mqtt.will.qos_level           MQTT QoS level for the will message.
 * \param mqtt.will.retained            If the will message should be retained by the broker.
 * \param mqtt.will.topic               Topic the will message should be publised to.
 */
GOS_CMD_CREATE_GETTER_SETTER(mqtt, auto_reconnect,      "mqtt.auto_reconnect",      SC3('m', 'q', 'a', 'r'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, broker_hostname,     "mqtt.broker_hostname",     SC3('m', 'q', 'b', 'h'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, broker_port,         "mqtt.broker_port",         SC3('m', 'q', 'b', 'p'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, clean_session,       "mqtt.clean_session",       SC3('m', 'q', 'c', 's'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, client_id,           "mqtt.client_id",           SC3('m', 'q', 'c', 'i'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, keep_alive_interval, "mqtt.keep_alive_interval", SC3('m', 'q', 'k', 'a'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, password,            "mqtt.password",            SC3('m', 'q', 'p', 'w'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, username,            "mqtt.username",            SC3('m', 'q', 'u', 'n'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, use_tls,             "mqtt.use_tls",             SC3('m', 'q', 'u', 't'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, mqtt_version,        "mqtt.version",             SC3('m', 'q', 'v', 'e'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, will_message,        "mqtt.will.message",        SC3('m', 'q', 'w', 'm'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, will_qos_level,      "mqtt.will.qos_level",      SC3('m', 'q', 'w', 'q'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, will_retained,       "mqtt.will.retained",       SC3('m', 'q', 'w', 'r'), 0, 1);
GOS_CMD_CREATE_GETTER_SETTER(mqtt, will_topic,          "mqtt.will.topic",          SC3('m', 'q', 'w', 't'), 0, 1);


// Setter and getter implementations

/******************************************************************************
 * Auto reconnect
 */
GOS_DEFINE_GETTER(mqtt, auto_reconnect)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%d", mqtt_configuration->automatic_reconnect);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, auto_reconnect)
{
    return gos_cmd_set_bool(argc, argv, &(mqtt_configuration->automatic_reconnect));
}

/******************************************************************************
 * Broker port
 */
GOS_DEFINE_GETTER(mqtt, broker_port)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%d", mqtt_configuration->broker_port);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, broker_port)
{
    GOS_CMD_PARSE_INT_ARG_WITH_VAR(uint16_t, mqtt_configuration->broker_port, argv[1], 0, 65535);
    return GOS_CMD_SET_OK;
}

/******************************************************************************
 * Clean session
 */
GOS_DEFINE_GETTER(mqtt, clean_session)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%d", mqtt_configuration->clean_session);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, clean_session)
{
    return gos_cmd_set_bool(argc, argv, &(mqtt_configuration->clean_session));
}

/******************************************************************************
 * Keep alive interval
 */
GOS_DEFINE_GETTER(mqtt, keep_alive_interval)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%d", mqtt_configuration->keep_alive_interval_seconds);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, keep_alive_interval)
{
    GOS_CMD_PARSE_INT_ARG_WITH_VAR(uint16_t, mqtt_configuration->keep_alive_interval_seconds, argv[1], 0, 65535);
    return GOS_CMD_SET_OK;
}

/******************************************************************************
 * Use TLS
 */
GOS_DEFINE_GETTER(mqtt, use_tls)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%d", mqtt_configuration->use_tls);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, use_tls)
{
    return gos_cmd_set_bool(argc, argv, &(mqtt_configuration->use_tls));
}

/******************************************************************************
 * Retaining will message
 */
GOS_DEFINE_GETTER(mqtt, will_retained)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%d", mqtt_will->retained);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, will_retained)
{
    return gos_cmd_set_bool(argc, argv, &(mqtt_will->retained));
}

/******************************************************************************
 * Will QoS level
 */
GOS_DEFINE_GETTER(mqtt, will_qos_level)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%d", mqtt_will->qos_level);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, will_qos_level)
{
    GOS_CMD_PARSE_INT_ARG_WITH_VAR(uint8_t, mqtt_will->qos_level, argv[1], 0, 2);
    return GOS_CMD_SET_OK;
}

/******************************************************************************
 * Broker hostname/ip address
 */
GOS_DEFINE_GETTER(mqtt, broker_hostname)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%s", mqtt_configuration->broker_hostname);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, broker_hostname)
{
    if(strlen(argv[1]) > sizeof(mqtt_configuration->broker_hostname))
    {
       GOS_LOG("Failed (maximum length is %u)", sizeof(mqtt_configuration->broker_hostname));
       return GOS_CMD_BAD_ARGS;
    }
    else
    {
       strcpy((char*)mqtt_configuration->broker_hostname, argv[1]);
       return GOS_CMD_SET_OK;
    }
}

/******************************************************************************
 * MQTT client ID
 */
GOS_DEFINE_GETTER(mqtt, client_id)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%s", mqtt_configuration->client_id);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, client_id)
{
    if(strlen(argv[1]) > sizeof(mqtt_configuration->client_id))
    {
       GOS_LOG("Failed (maximum length is %u)", sizeof(mqtt_configuration->client_id));
       return GOS_CMD_BAD_ARGS;
    }
    else
    {
       strcpy((char*)mqtt_configuration->client_id, argv[1]);
       return GOS_CMD_SET_OK;
    }
}

/******************************************************************************
 * MQTT broker connection password (username & password are optional)
 */
GOS_DEFINE_GETTER(mqtt, password)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%s", mqtt_configuration->password);
    return GOS_CMD_SUCCESS;
}


GOS_DEFINE_SETTER(mqtt, password)
{
    if(strlen(argv[1]) > sizeof(mqtt_configuration->password))
    {
       GOS_LOG("Failed (maximum length is %u)", sizeof(mqtt_configuration->password));
       return GOS_CMD_BAD_ARGS;
    }
    else
    {
       strcpy((char*)mqtt_configuration->password, argv[1]);
       return GOS_CMD_SET_OK;
    }
}

/******************************************************************************
 * MQTT broker connection username (username & password are optional)
 */
GOS_DEFINE_GETTER(mqtt, username)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%s", mqtt_configuration->username);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, username)
{
    if(strlen(argv[1]) > sizeof(mqtt_configuration->username))
    {
       GOS_LOG("Failed (maximum length is %u)", sizeof(mqtt_configuration->username));
       return GOS_CMD_BAD_ARGS;
    }
    else
    {
       strcpy((char*)mqtt_configuration->username, argv[1]);
       return GOS_CMD_SET_OK;
    }
}

/******************************************************************************
 * MQTT will message (a will is optional)
 */
GOS_DEFINE_GETTER(mqtt, will_message)
{
    // There is no guarantee by the library that the payload
    // is a null-terminated string. However the setter requires it
    // hence the payload will be a null terminated string
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%s", settings->will_message);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, will_message)
{
    if(strlen(argv[1]) > sizeof(settings->will_message))
    {
       GOS_LOG("Failed (maximum length is %u)", sizeof(settings->will_message));
       return GOS_CMD_BAD_ARGS;
    }
    else
    {
       strcpy((char*)settings->will_message, argv[1]);
       mqtt_will->payload = (uint8_t*) settings->will_message;
       mqtt_will->payload_length = strlen(argv[1]);
       return GOS_CMD_SET_OK;
    }
}

/******************************************************************************
 * MQTT will topic (a will is optional)
 */
GOS_DEFINE_GETTER(mqtt, will_topic)
{
    gos_cmd_format_response(GOS_CMD_SUCCESS, "%s", mqtt_will->topic);
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, will_topic)
{
    if(strlen(argv[1]) > sizeof(mqtt_will->topic))
    {
       GOS_LOG("Failed (maximum length is %u)", sizeof(mqtt_will->topic));
       return GOS_CMD_BAD_ARGS;
    }
    else
    {
       strcpy((char*)mqtt_will->topic, argv[1]);
       return GOS_CMD_SET_OK;
    }
}

/******************************************************************************
 * MQTT version (either '3.1' or '3.1.1' is supported)
 */
GOS_DEFINE_GETTER(mqtt, mqtt_version)
{
    if (mqtt_configuration->mqtt_version == MQTT_VERSION_3_1)
    {
        gos_cmd_format_response(GOS_CMD_SUCCESS, "3.1");
    }
    else if (mqtt_configuration->mqtt_version == MQTT_VERSION_3_1_1)
    {
        gos_cmd_format_response(GOS_CMD_SUCCESS, "3.1.1");
    }
    
    return GOS_CMD_SUCCESS;
}

GOS_DEFINE_SETTER(mqtt, mqtt_version)
{
    if(strcmp(argv[1], "3.1") == 0)
    {
       mqtt_configuration->mqtt_version = MQTT_VERSION_3_1;
       return GOS_CMD_SET_OK;
    }
    else if(strcmp(argv[1], "3.1.1") == 0)
    {
       mqtt_configuration->mqtt_version = MQTT_VERSION_3_1_1;
       return GOS_CMD_SET_OK;
    }
    else 
    {
       GOS_LOG("Failed, valid options are: '3.1', '3.1.1'");
       return GOS_CMD_BAD_ARGS;
    }
}
