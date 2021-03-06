/*! 
 * --------------------------------------------------------------------------------
 *
 * \file	rpi_command_handler.c
 * \brief
 * \author	sebastian lesse
 *
 * --------------------------------------------------------------------------------
 */

#define TRACER_OFF

#ifdef TRACER_ON
#warning __WARNING__TRACER_ENABLED__WARNING__
#endif

// --------------------------------------------------------------------------------

#include "config.h"

// --------------------------------------------------------------------------------

#include "tracer.h"

// --------------------------------------------------------------------------------

#include "command_management/command_handler_interface.h"
#include "command_management/command_buffer_interface.h"
#include "command_management/command_handler_interface.h"
#include "command_management/answer_buffer_interface.h"
#include "command_management/protocol_interface.h"

#include "common/local_context.h"
#include "common/signal_slot_interface.h"

#include "io_management/io_input_controller.h"
#include "io_management/io_output_controller.h"

#include "command_management/command_controller.h"

#include "command_handler/rpi_command_handler.h"

// --------------------------------------------------------------------------------

extern COMMAND_TABLE_INTERFACE rpi_cmd_handler_table[];
extern u8 rpi_cmd_handler_table_get_size(void);

// --------------------------------------------------------------------------------

static COMMAND_HANDLER_INTERFACE rpi_command_handler = {
	&rpi_cmd_handler_init, 			
	&rpi_cmd_handler_set_request,		
	&rpi_cmd_handler_set_unrequested,
	&rpi_cmd_handler_get_protocol,
	&rpi_cmd_handler_is_requested,		
	&rpi_cmd_handler_get_command_code,
	&rpi_cmd_handler_table_get_size,
	rpi_cmd_handler_table, 			
	&rpi_cmd_default_handler,
	0 					
};

// --------------------------------------------------------------------------------

static void rpi_command_handler_slot_cmd_received_CALLBACK(const void* p_arg) {
	rpi_cmd_handler_set_request((PROTOCOL_INTERFACE*)p_arg);
}

// --------------------------------------------------------------------------------

SIGNAL_SLOT_INTERFACE_CREATE_SLOT(RPI_PROTOCOL_COMMAND_RECEIVED_SIGNAL, RPI_CMD_HANDLER_SLOT_CMD_RECEIVED, rpi_command_handler_slot_cmd_received_CALLBACK)

// --------------------------------------------------------------------------------

static PROTOCOL_INTERFACE* p_act_protocol;

// --------------------------------------------------------------------------------

void rpi_cmd_handler_init(void) {

	DEBUG_PASS("rpi_cmd_handler_init()");

	p_act_protocol = 0;
	RPI_CMD_HANDLER_SLOT_CMD_RECEIVED_connect();

	command_controller_register_handler(&rpi_command_handler);
}

void rpi_cmd_handler_set_request(PROTOCOL_INTERFACE* p_protocol_handler) {

	DEBUG_TRACE_byte(p_protocol_handler->command_code, "rpi_cmd_handler_set_request() - cmd_code");
	p_act_protocol = p_protocol_handler;
}

PROTOCOL_INTERFACE* rpi_cmd_handler_get_protocol(void) {
	return p_act_protocol;
}

void rpi_cmd_handler_set_unrequested(void) {

	DEBUG_PASS("rpi_cmd_handler_set_unrequested()");
	p_act_protocol = 0;
}


u8 rpi_cmd_handler_is_requested(void) {

	if (p_act_protocol == 0) {
		return 0;
	}

	DEBUG_PASS("rpi_cmd_default_handler()");
	return 1;
}

u8 rpi_cmd_handler_get_command_code(void) {

	if (p_act_protocol != 0) {
		return p_act_protocol->command_code;
	} else {
		return 0;
	}
}

u8 rpi_cmd_default_handler(PROTOCOL_INTERFACE* p_protocol) {

	DEBUG_PASS("rpi_cmd_default_handler()");

	p_protocol->set_finished(CMD_ERR_INVALID_COMMAND);
	return CMD_ERR_INVALID_COMMAND;
}
