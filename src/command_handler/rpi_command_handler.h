/*! \file *********************************************************************

 *****************************************************************************/

#ifndef RPI_COMMAND_HANDLER_H_
#define RPI_COMMAND_HANDLER_H_

#include "config.h"  // immer als erstes einbinden!

#include "command_management/command_handler_interface.h"


#define RPI_COMMAND_GET_VERSION			0x01
#define RPI_COMMAND_GET_INPUT_LIST		0x02
#define RPI_COMMAND_GET_OUTPUT_LIST		0x03
#define RPI_COMMAND_SET_OUTPUT			0x04
#define RPI_COMMAND_GET_OUTPUT_STATE		0x05
#define RPI_COMMAND_GET_INPUT_STATE		0x06
#define RPI_COMMAND_GET_TEMPERATURE		0x07
#define RPI_COMMAND_GET_HUMIDTY			0x08
#define RPI_COMMAND_GET_ADC			0x09
#define RPI_COMMAND_GET_LIGHT			0x0A
#define RPI_COMMAND_IR_REMOTE			0x0B


/*!
 *
 */
void rpi_cmd_handler_init(void);

/*!
 *
 * @param cmd_code
 */
void rpi_cmd_handler_set_request(PROTOCOL_INTERFACE* p_protocol_handler);

/*!
 *
 * @return
 */
PROTOCOL_INTERFACE* rpi_cmd_handler_get_protocol(void);

/*!
 *
 */
void rpi_cmd_handler_set_unrequested(void);

/*!
 *
 * @return
 */
u8 rpi_cmd_handler_get_command_code(void);

/*!
 *
 * @return
 */
u8 rpi_cmd_handler_is_requested(void);

/*!
 *
 * @return
 */
u8 rpi_cmd_default_handler(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @param p_cmd_buffer
 * @param p_answ_buffer
 * @return
 */
u8 rpi_cmd_get_version(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @return
 */
u8 rpi_cmd_get_input_list(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @return
 */
u8 rpi_cmd_get_output_list(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @return
 */
u8 rpi_cmd_set_output(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @return
 */
u8 rpi_cmd_get_input_state(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @return
 */
u8 rpi_cmd_get_output_state(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @return
 */
u8 rpi_cmd_get_temperature(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @return
 */
u8 rpi_cmd_get_humidity(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @return
 */
u8 rpi_cmd_get_adc(PROTOCOL_INTERFACE* p_protocol);

/*!
 *
 * @return
 */
u8 rpi_cmd_get_light(PROTOCOL_INTERFACE* p_protocol);

#endif //RPI_COMMAND_HANDLER_H_
