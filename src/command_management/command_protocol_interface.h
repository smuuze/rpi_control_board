/*! 
 * --------------------------------------------------------------------------------
 *
 * \file	command_protocol_interface.h
 * \author	sebastian lesse
 *
 * --------------------------------------------------------------------------------
 */

#ifndef _COMMAND_PROTOCOL_INTERFACE_H_
#define _COMMAND_PROTOCOL_INTERFACE_H_

// --------------------------------------------------------------------------------

#include "command_buffer_interface.h"
#include "answer_buffer_interface.h"

// --------------------------------------------------------------------------------

typedef void (*CMD_PROTOCOL_INTERFACE_INITIALIZATION_CALLBACK)	(void);
typedef u8 (*CMD_PROTOCOL_INTERFACE_REQUESTED_CALLBACK)		(void);
typedef u8 (*CMD_PROTOCOL_INTERFACE_GET_COMMAND_CODE_CALLBACK)	(void);
typedef void (*CMD_PROTOCOL_INTERFACE_SET_ERROR_CODE_CALLBACK)	(u8);

// --------------------------------------------------------------------------------

/*!
 *
 */
typedef enum CMD_PROTOCOL_ROLE_TYPE {
	ROLE_TYPE_HOST,//!< ROLE_TYPE_HOST
	ROLE_TYPE_SLAVE
} CMD_PROTOCOL_ROLE_TYPE;

/*!
 *
 */
typedef enum CMD_PROTOCOL_HW_INTERFACE {
	HW_INTERFACE_SPI_0, //!< HW_INTERFACE_SPI_0
	HW_INTERFACE_SPI_1, //!< HW_INTERFACE_SPI_1
	HW_INTERFACE_UART_0,//!< HW_INTERFACE_UART_0
	HW_INTERFACE_UART_1,//!< HW_INTERFACE_UART_1
	HW_INTERFACE_I2C_0, //!< HW_INTERFACE_I2C_0
	HW_INTERFACE_I2C_1  //!< HW_INTERFACE_I2C_1
} CMD_PROTOCOL_HW_INTERFACE;

/*!
 *
 */
typedef struct COMMAND_PROTOCOL_INTERFACE {

	CMD_PROTOCOL_ROLE_TYPE					role;

	CMD_PROTOCOL_HW_INTERFACE				hw_interface;
	CMD_PROTOCOL_INTERFACE_INITIALIZATION_CALLBACK		init;
	CMD_PROTOCOL_INTERFACE_REQUESTED_CALLBACK		request_pending;
	CMD_PROTOCOL_INTERFACE_GET_COMMAND_CODE_CALLBACK 	get_cmd_code;
	CMD_PROTOCOL_INTERFACE_SET_ERROR_CODE_CALLBACK 		set_err_code;

	COMMAND_BUFFER_INTERFACE* 				p_cmd_buffer;
	ANSWER_BUFFER_INTERFACE* 				p_answ_buffer;

	struct COMMAND_PROTOCOL_INTERFACE*			next;

} COMMAND_PROTOCOL_INTERFACE;

// --------------------------------------------------------------------------------

#endif // _COMMAND_PROTOCOL_INTERFACE_H_
