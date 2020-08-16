/*! 
 * --------------------------------------------------------------------------------
 *
 * \file	ir_protocol_jvc_commands.c
 * \brief
 * \author	sebastian lesse
 *
 * --------------------------------------------------------------------------------
 */

#define TRACER_OFF

// --------------------------------------------------------------------------------

#include "config.h"

// --------------------------------------------------------------------------------

#include "tracer.h"

// --------------------------------------------------------------------------------

#include "3rdparty/ir_protocol/ir_protocol_jvc.h"

// --------------------------------------------------------------------------------

inline void ir_protocol_jvc_address_radio(JVC_IR_PROTOCOL_COMMAND_TYPE* p_command) {
	//p_command->address = 0b11100000;
}

// --------------------------------------------------------------------------------

inline void ir_protocol_jvc_cmd_radio_power(JVC_IR_PROTOCOL_COMMAND_TYPE* p_command) {
	DEBUG_PASS("ir_protocol_jvc_cmd_radio_power()");
	//p_command->control = 0b01000000;
}

inline void ir_protocol_jvc_cmd_radio_volume_up(JVC_IR_PROTOCOL_COMMAND_TYPE* p_command) {
	DEBUG_PASS("ir_protocol_jvc_cmd_radio_volume_up()");
	//p_command->control = 0b11100000;
}

inline void ir_protocol_jvc_cmd_radio_volume_down(JVC_IR_PROTOCOL_COMMAND_TYPE* p_command) {
	DEBUG_PASS("ir_protocol_jvc_cmd_radio_volume_down()");
	//p_command->control = 0b11010000;
}

inline void ir_protocol_jvc_cmd_radio_volume_mute(JVC_IR_PROTOCOL_COMMAND_TYPE* p_command) {
	DEBUG_PASS("ir_protocol_jvc_cmd_radio_volume_mute()");
	//p_command->control = 0b11110000;
}

inline void ir_protocol_jvc_cmd_radio_play(JVC_IR_PROTOCOL_COMMAND_TYPE* p_command) {
	DEBUG_PASS("ir_protocol_jvc_cmd_radio_play()");
	//p_command->control = 0b11100010;
}

inline void ir_protocol_jvc_cmd_radio_pause(JVC_IR_PROTOCOL_COMMAND_TYPE* p_command) {
	DEBUG_PASS("ir_protocol_jvc_cmd_radio_pause()");
	//p_command->control = 0b01010010;
}

inline void ir_protocol_jvc_cmd_radio_stop(JVC_IR_PROTOCOL_COMMAND_TYPE* p_command) {
	DEBUG_PASS("ir_protocol_jvc_cmd_radio_stop()");
	//p_command->control = 0b01100010;
}