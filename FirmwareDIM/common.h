/*
 * EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND
 * CONDITIONS OF THE CONTROLLING LICENSE AGREEMENT FOUND AT LICENSE.md
 * IN THIS SDK. IF YOU DO NOT AGREE TO THE LICENSE TERMS AND CONDITIONS,
 * PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.
 * (c) Copyright 2018, Silicon Laboratories Inc.  All rights reserved.
 */
#pragma once

#include "gos.h"

//To stringfication operation
#define str(s) #s

//Device type
//#define WTOM01 0x01
#define WDIM01 0x02
//#define WDIM01 0x03

//choose device for compilation
#define DEVICE_TYPE str(WDIM01)
#define HW_VERSION  str(0.0.2)

extern gos_timer_t timer;
extern uint16_t dimming_ant;

//HTTP Errors
#define ERROR_405 "<h1>Invalid Request Method. Must be POST</h1>"
#define ERROR_500 "<h1>Failed to read JSON data</h1>"
#define ERROR_400 "<h1>Failed to parse JSON</h1>"


gos_result_t json_parser_request   (const gos_hs_request_t *request, const char *arg);
gos_result_t get_type_request   (const gos_hs_request_t *request, const char *arg);


extern char ip_str_buffer[32];
