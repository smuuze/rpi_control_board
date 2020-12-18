/*! 
 * --------------------------------------------------------------------------------
 *
 * \file	ui/log_interface/log_interface.c
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

#include "mcu_task_management/mcu_task_interface.h"
#include "common/local_module_status.h"
#include "common/signal_slot_interface.h"
#include "common/common_tools_string.h"
#include "common/qeue_interface.h"

// --------------------------------------------------------------------------------

#include "ui/file_interface/file_interface.h"
#include "ui/log_interface/log_interface.h"

// --------------------------------------------------------------------------------

#ifndef LOG_INTERFACE_MAX_MESSAGE_LENGTH
#define LOG_INTERFACE_MAX_MESSAGE_LENGTH				255
#endif

#ifndef LOG_INTERFACE_QEUE_SIZE
#define LOG_INTERFACE_QEUE_SIZE						50
#endif

#ifndef LOG_INTERFACE_TASK_RUN_INTERVAL_MS
#define LOG_INTERFACE_TASK_RUN_INTERVAL_MS				5
#endif

// --------------------------------------------------------------------------------

#define LOG_INTERFACE_STATUS_QEUE_OVERFLOW				(1 << 0)
#define LOG_INTERFACE_STATUS_USER_CFG_COMPLETE				(1 << 1)

BUILD_MODULE_STATUS_U8(LOG_INTERFACE_STATUS)

// --------------------------------------------------------------------------------

typedef enum {
	LOG_INTERFACE_TASK_STATE_START_UP,
	LOG_INTERFACE_TASK_STATE_IDLE,
	LOG_INTERFACE_TASK_STATE_OPEN_FILE,
	LOG_INTERFACE_TASK_STATE_CLOSE_FILE,
	LOG_INTERFACE_TASK_STATE_WRITE_LOG
} LOG_INTERFACE_TASK_STATE_TYPE;

// --------------------------------------------------------------------------------

/*
 *
 */
static void log_interface_new_cfg_object_SLOT_CALLBACK(const void* p_argument);

/*
 *
 */
static void log_interface_cfg_complete_SLOT_CALLBACK(const void* p_argument);

// --------------------------------------------------------------------------------

SIGNAL_SLOT_INTERFACE_CREATE_SLOT(CFG_PARSER_CFG_COMPLETE_SIGNAL, LOG_INTERFACE_CFG_COMPLETE_SLOT, log_interface_cfg_complete_SLOT_CALLBACK)
SIGNAL_SLOT_INTERFACE_CREATE_SLOT(CFG_PARSER_NEW_CFG_OBJECT_SIGNAL, LOG_INTERFACE_NEW_CFG_OBJECT_SLOT, log_interface_new_cfg_object_SLOT_CALLBACK)

// --------------------------------------------------------------------------------

FILE_INTERFACE_CREATE_FILE(LOG_FILE)

// --------------------------------------------------------------------------------

QEUE_INTERFACE_BUILD_QEUE(LOG_QEUE, char, LOG_INTERFACE_MAX_MESSAGE_LENGTH, LOG_INTERFACE_QEUE_SIZE)

// --------------------------------------------------------------------------------

/*
 *
 */
static void log_interface_task_init(void);

/*
 *
 */
static u16 log_interface_task_get_schedule_interval(void);

/*
 *
 */
static MCU_TASK_INTERFACE_TASK_STATE log_interface_task_get_state(void);

/*
 *
 */
static void log_interface_task_run(void);

/*
 *
 */
static void log_interface_task_background_run(void);

/*
 *
 */
static void log_interface_task_sleep(void);

/*
 *
 */
static void log_interface_task_wakeup(void);

/*
 *
 */
static void log_interface_task_finish(void);

/*
 *
 */
static void log_interface_task_terminate(void);

// --------------------------------------------------------------------------------

static MCU_TASK_INTERFACE log_interface_task = {

	0, 						// u8 identifier,
	0, 						// u16 new_run_timeout,
	0, 						// u16 last_run_time,
	&log_interface_task_init, 			// MCU_TASK_INTERFACE_INIT_CALLBACK			init,
	&log_interface_task_get_schedule_interval,	// MCU_TASK_INTERFACE_INIT_CALLBACK			init,
	&log_interface_task_get_state, 			// MCU_TASK_INTERFACE_GET_STATE_CALLBACK	get_sate,
	&log_interface_task_run, 			// MCU_TASK_INTERFACE_RUN_CALLBACK		run,
	&log_interface_task_background_run,		// MCU_TASK_INTERFACE_BG_RUN_CALLBACK		background_run,
	&log_interface_task_sleep, 			// MCU_TASK_INTERFACE_SLEEP_CALLBACK		sleep,
	&log_interface_task_wakeup, 			// MCU_TASK_INTERFACE_WAKEUP_CALLBACK		wakeup,
	&log_interface_task_finish, 			// MCU_TASK_INTERFACE_FINISH_CALLBACK		finish,
	&log_interface_task_terminate, 			// MCU_TASK_INTERFACE_TERMINATE_CALLBACK	terminate,
	0						// next-task
};

// --------------------------------------------------------------------------------

/*
 *
 */
static char cfg_file_path[LOG_INTERFACE_MAX_LINE_LENGTH];

/*
 *
 */
static LOG_INTERFACE_TASK_STATE_TYPE app_task_state = LOG_INTERFACE_TASK_STATE_IDLE;

// --------------------------------------------------------------------------------

void log_interface_init(void) {

	LOG_INTERFACE_STATUS_clear_all();

	LOG_QEUE_init();

	DEBUG_PASS("log_interface_init() - LOG_INTERFACE_CFG_COMPLETE_SLOT_connect()");
	LOG_INTERFACE_CFG_COMPLETE_SLOT_connect();

	DEBUG_PASS("log_interface_init() - LOG_INTERFACE_NEW_CFG_OBJECT_SLOT_connect()");
	LOG_INTERFACE_NEW_CFG_OBJECT_SLOT_connect();

	DEBUG_PASS("log_interface_init() - mcu_task_controller_register_task()");
	mcu_task_controller_register_task(&log_interface_task);
}

// --------------------------------------------------------------------------------

static void log_interface_task_init(void) {
	
	DEBUG_PASS("log_interface_task_init()");
	app_task_state = LOG_INTERFACE_TASK_STATE_START_UP;
}

static u16 log_interface_task_get_schedule_interval(void) {

	return LOG_INTERFACE_TASK_RUN_INTERVAL_MS;
}

static MCU_TASK_INTERFACE_TASK_STATE log_interface_task_get_state(void) {

	if (LOG_QEUE_is_empty() == 0) {
		DEBUG_PASS("log_interface_task_get_state() - Log-Messages pending");
		return MCU_TASK_RUNNING;
	}
	
	return MCU_TASK_SLEEPING;
}

static void log_interface_task_run(void) {
	
	DEBUG_PASS("log_interface_task_run()");

	switch (app_task_state) {
		
		default:

			app_task_state = LOG_INTERFACE_TASK_STATE_START_UP;
			// no break;

		case LOG_INTERFACE_TASK_STATE_START_UP:

			if (LOG_INTERFACE_STATUS_is_set(LOG_INTERFACE_STATUS_USER_CFG_COMPLETE) == 0) {
				break;
			}
	
			app_task_state = LOG_INTERFACE_TASK_STATE_IDLE;
			break;

		case LOG_INTERFACE_TASK_STATE_IDLE :

			if (LOG_QEUE_is_empty()) {
				break;
			}

			DEBUG_PASS("log_interface_task_run() - LOG_INTERFACE_TASK_STATE_IDLE > LOG_INTERFACE_TASK_STATE_OPEN_FILE");
			app_task_state = LOG_INTERFACE_TASK_STATE_OPEN_FILE;
			// no break;

		case LOG_INTERFACE_TASK_STATE_OPEN_FILE :

			if (LOG_FILE_open_path(cfg_file_path) == 0) {
				DEBUG_PASS("log_interface_task_run() - Open new cfg-file has FAILED !!! ---");
				app_task_state = LOG_INTERFACE_TASK_STATE_OPEN_FILE;
				break;
			}

			DEBUG_PASS("log_interface_task_run() - LOG_INTERFACE_TASK_STATE_OPEN_FILE > LOG_INTERFACE_TASK_STATE_LOAD_CONFIGURATION");

			app_task_state = LOG_INTERFACE_TASK_STATE_WRITE_LOG;
			// no break;
	

		case LOG_INTERFACE_TASK_STATE_WRITE_LOG :

			log_interface_parse_configuration_file();

			DEBUG_PASS("log_interface_task_run() - LOG_INTERFACE_TASK_STATE_LOAD_CONFIGURATION > LOG_INTERFACE_TASK_STATE_IDLE");
			app_task_state = LOG_INTERFACE_TASK_STATE_IDLE;

			break;

		case LOG_INTERFACE_TASK_STATE_CLOSE_FILE:

			LOG_FILE_close();

			DEBUG_PASS("log_interface_task_run() - LOG_INTERFACE_TASK_STATE_LOAD_CONFIGURATION > LOG_INTERFACE_TASK_STATE_IDLE");
			app_task_state = LOG_INTERFACE_TASK_STATE_IDLE;

			break;
	}

}

static void log_interface_task_background_run(void) {

}

static void log_interface_task_sleep(void) {

}

static void log_interface_task_wakeup(void) {

}

static void log_interface_task_finish(void) {

}

static void log_interface_task_terminate(void) {

}

// --------------------------------------------------------------------------------

static void log_interface_new_cfg_object_SLOT_CALLBACK(const void* p_argument) {

}

static void log_interface_cfg_complete_SLOT_CALLBACK(const void* p_argument) {
	LOG_INTERFACE_STATUS_set(LOG_INTERFACE_STATUS_USER_CFG_COMPLETE);
}

// --------------------------------------------------------------------------------

void log_message(const char* message) {

}