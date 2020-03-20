#-----------------------------------------------------------------------------
#       Makefile fuer AVR-GCC Projekte
#-----------------------------------------------------------------------------

include $(MAKE_PATH)/make_cpu.mk

# -----------------------------------------------------------------------
# Definitions: [-D name[=definition]...] [-U name...]
# Things that might be added to DEFS:
#   BOARD             Board used: see $(BRDS_PATH)/board.h
#   EXT_BOARD         Extension board used (if any): see $(BRDS_PATH)/board.h
DEFS += -D BOARD=$(BOARD)
DEFS += -D BOARD_ID=$(BOARD_ID)

DEFS += -D VERSION_MAJOR=$(VERSION_MAJOR)
DEFS += -D VERSION_MINOR=$(VERSION_MINOR)

# -----------------------------------------------------------------------

BASE_PATH	?= ..
APP_PATH	?= $(BASE_PATH)/src
MAKE_PATH	?= $(BASE_PATH)/make

# -----------------------------------------------------------------------
# Include path

INC_PATH 	+= $(APP_PATH)
INC_PATH 	+= .

# ---- DEFAULT APP CONFIGURATION ----

APP_TASK_LIST	?= 

# -----------------------------------------------------------------------
BOARD_INC_PATH = ${APP_PATH}/platine
INC_PATH += $(BOARD_INC_PATH)

CSRCS += ${BOARD_INC_PATH}/board_${BOARD_ID}.c


INC_PATH += $(APP_PATH)

# ---- COMMON MODULES ---------------------------------------------------
COMMON_INC_PATH = $(APP_PATH)/common
INC_PATH += $(COMMON_INC_PATH)

CSRCS += $(COMMON_INC_PATH)/local_module_status.c
CSRCS += $(COMMON_INC_PATH)/local_mutex.c
CSRCS += $(COMMON_INC_PATH)/local_progmem_load.c
CSRCS += $(COMMON_INC_PATH)/local_data_storage_array.c
CSRCS += $(COMMON_INC_PATH)/local_msg_buffer.c
CSRCS += $(COMMON_INC_PATH)/signal_slot_interface.c
CSRCS += $(COMMON_INC_PATH)/math_module.c

# Projekt-Spezifische Quelldateien --------------------------------------

CSRCS += $(APP_PATH)/specific.c

# ---- SYSTEM INTERFACE -------------------------------------------------

SYSTEM_INC_PATH = $(APP_PATH)/system
INC_PATH += $(SYSTEM_INC_PATH)

CSRCS += $(SYSTEM_INC_PATH)/system_interface.c

# ---- TRACER -----------------------------------------------------------

include $(MAKE_PATH)/make_tracer.mk

# ---- HOST-INTERFACE ---------------------------------------------------

ifneq '' '$(findstring SPI,$(HOST_INTERFACE_TYPE))'
	DEFS += -D HOST_INTERFACE_SPI=1
	DRIVER_MODULE_CFG += SPI0
else
ifneq '' '$(findstring HOST_INTERFACE_TYPE,$(APP_TASK_CFG))'
	DEFS += -D HOST_INTERFACE_USART=1
	DRIVER_MODULE_CFG += USART0
endif
endif

# -----------------------------------------------------------------------

COMMAND_MANAGEMENT_INC_PATH = $(APP_PATH)/command_management
INC_PATH += $(COMMAND_MANAGEMENT_INC_PATH)

CSRCS += $(COMMAND_MANAGEMENT_INC_PATH)/command_controller.c

# -----------------------------------------------------------------------

COMMAND_HANDLER_INC_PATH = $(APP_PATH)/command_handler
INC_PATH += $(COMMAND_HANDLER_INC_PATH)

CSRCS += $(COMMAND_HANDLER_INC_PATH)/rpi_command_handler.c

# -----------------------------------------------------------------------

TIME_MANAGEMENT_INC_PATH = $(APP_PATH)/time_management
INC_PATH += $(TIME_MANAGEMENT_INC_PATH)

CSRCS += $(TIME_MANAGEMENT_INC_PATH)/time_management.c

# ---- TASK MANAGEMENT -------------------------------------------------------------------

MCU_TASK_MANAGEMENT_INC_PATH = $(APP_PATH)/mcu_task_management
INC_PATH += $(MCU_TASK_MANAGEMENT_INC_PATH)

APP_TASK_INC_PATH = $(APP_PATH)/app_tasks
INC_PATH += $(APP_TASK_INC_PATH)

CSRCS += $(MCU_TASK_MANAGEMENT_INC_PATH)/mcu_task_controller.c
CSRCS += $(MCU_TASK_MANAGEMENT_INC_PATH)/mcu_idle_task.c

ifneq '' '$(findstring EVENT,$(APP_TASK_CFG))'
DEFS  += -D HAS_APP_TASK_EVENT=1
CSRCS += $(APP_TASK_INC_PATH)/local_event_task.c
endif

CSRCS += $(APP_TASK_INC_PATH)/local_cmd_mcu_task.c

ifneq '' '$(findstring LED_MATRIX,$(APP_TASK_CFG))'
DEFS  += -D HAS_APP_TASK_LED_MATRIX=1
CSRCS += $(APP_TASK_INC_PATH)/local_led_mcu_task.c
endif

ifneq '' '$(findstring DEBUS,$(APP_TASK_CFG))'
DEFS += -D HAS_APP_TASK_DEBUS=1
CSRCS += $(APP_TASK_INC_PATH)/local_debus_mcu_task.c
endif

ifneq '' '$(findstring TEST_TRACER,$(APP_TASK_CFG))'
DEFS += -D HAS_APP_TASK_TEST_TRACER=1
CSRCS += $(APP_TASK_INC_PATH)/test_tracer_mcu_task.c
endif

ifneq '' '$(findstring IR_REMOTE,$(APP_TASK_CFG))'
DEFS += -D HAS_APP_TASK_IR_REMOTE=1
CSRCS += $(APP_TASK_INC_PATH)/ir_remote_mcu_task.c
endif



# ---- EXPANSION BOARDS -------------------------------------------------------------------

EXPANSION_BOARD_PATH = $(APP_PATH)/expansion
INC_PATH += $(EXPANSION_BOARD_PATH)

ifneq '' '$(findstring SENSOR_SHT31_ADS1115,$(EXPANSION_BOARD_CFG))'
	DEFS  += -D HAS_EXPANSION_BOARD_SENSOR_SHT31_ADS1115=1
	
	CSRCS += $(EXPANSION_BOARD_PATH)/local_ads1115_mcu_task.c
	CSRCS += $(EXPANSION_BOARD_PATH)/local_sht31_mcu_task.c
endif

ifneq '' '$(findstring GPIO_PCA9670,$(EXPANSION_BOARD_CFG))'
	DEFS  += -D HAS_EXPANSION_BOARD_GPIO_PCA9670=1
	CSRCS += $(EXPANSION_BOARD_PATH)/driver_PCA9670.c
endif

# ---- 3rd PARTY MODULES ------------------------------------------------------------------

include $(MAKE_PATH)/make_third_party.mk

# ---- SENSOR_UNITS -----------------------------------------------------------------------

include $(MAKE_PATH)/make_sensor.mk

# ---- MANAGEMENT MODULES -----------------------------------------------------------------
	
ifneq '' '$(findstring POWER,$(MANAGEMENT_MODULE_CFG))'
	POWER_MANAGEMENT_PATH = $(APP_PATH)/power_management
	CSRCS += $(POWER_MANAGEMENT_PATH)/power_management.c
endif

ifneq '' '$(findstring IO,$(MANAGEMENT_MODULE_CFG))'
	DEFS  += -D HAS_MANAGEMENT_MODULE_IO=1

	IO_CONTROLLER_INC_PATH = $(APP_PATH)/io_management
	INC_PATH += $(IO_CONTROLLER_INC_PATH)

	CSRCS += $(IO_CONTROLLER_INC_PATH)/io_output_controller.c
	CSRCS += $(IO_CONTROLLER_INC_PATH)/io_input_controller.c
endif

ifneq '' '$(findstring RPI_PROTOCOL,$(MANAGEMENT_MODULE_CFG))'
	DEFS  += -D HAS_MANAGEMENT_MODULE_RPI_PROTOCOL=1
	PROTOCOL_MANAGEMENT_PATH = $(APP_PATH)/protocol_management
	CSRCS += $(PROTOCOL_MANAGEMENT_PATH)/rpi_protocol_handler.c
endif

# -----------------------------------------------------------------------


INITIALIZATION_INC_PATH = $(APP_PATH)/initialization
INC_PATH += $(INITIALIZATION_INC_PATH)

CSRCS += $(INITIALIZATION_INC_PATH)/sensor_initialization.c
CSRCS += $(INITIALIZATION_INC_PATH)/button_initialization.c
CSRCS += $(INITIALIZATION_INC_PATH)/output_initialization.c
CSRCS += $(INITIALIZATION_INC_PATH)/protocol_initialization.c
CSRCS += $(INITIALIZATION_INC_PATH)/command_initialization.c
CSRCS += $(INITIALIZATION_INC_PATH)/task_initialization.c
CSRCS += $(INITIALIZATION_INC_PATH)/system_initialization.c
CSRCS += $(INITIALIZATION_INC_PATH)/power_initialization.c
CSRCS += $(INITIALIZATION_INC_PATH)/initialization.c

# ---- DRIVER MODULES ---------------------------------------------------

include $(MAKE_PATH)/make_driver.mk

# -----------------------------------------------------------------------
# Library path
LIB_PATH +=

# -----------------------------------------------------------------------
# Libraries to link with the project
LIBS +=

# -----------------------------------------------------------------------
# Options to request or suppress warnings: [-fsyntax-only] [-pedantic[-errors]] [-w] [-Wwarning...]
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
WARNINGS += -Wall
WARNINGS += -Wstrict-prototypes


# -----------------------------------------------------------------------
# Options that control optimization: [-O[0|1|2|3|s]]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
# OPTIMIZATION  += -O3 -mcall-prologues
OPTIMIZATION  += -Os
OPTIMIZATION += -mcall-prologues
OPTIMIZATION += -ffunction-sections
OPTIMIZATION += -fshort-enums
OPTIMIZATION += -funsigned-char
OPTIMIZATION += -funsigned-bitfields
#OPTIMIZATION += -fpack-struct
#OPTIMIZATION += -mshort-calls

# -----------------------------------------------------------------------
# Extra flags to use when preprocessing
PP_EXTRA_FLAGS +=

# Extra flags to use when compiling
C_EXTRA_FLAGS +=

# Extra flags to use when assembling
AS_EXTRA_FLAGS +=

# Extra flags to use when linking
LD_EXTRA_FLAGS += -Wl,--gc-sections,--relax
#-Wl,--gc-sections

# -----------------------------------------------------------------------
include $(MAKE_PATH)/make_toolchain.mk
include $(MAKE_PATH)/make_avr_dude.mk
include $(MAKE_PATH)/make_targets.mk

# -----------------------------------------------------------------------
# Debug-Ausgabe aller Variablen
#$(foreach var,$(.VARIABLES), $(info $(var) : $(value $(var)) )  )
