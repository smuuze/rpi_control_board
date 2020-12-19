
ifeq ($(OS),Windows_NT)

	CROSS_COMPILER_PATH   = ../../../../bin/avr/winavr/20100110/bin
	CROSS_COMPILER_SUFFIX = .exe
    
else

	UNAME_S := $(shell uname -s)
    
	ifeq ($(UNAME_S),Linux)
		CROSS_COMPILER_PATH   = /usr/bin
	else
		ifeq ($(UNAME_S),Darwin)
		CCFLAGS += -D OSX

		ifeq ($(CROSS_COMPILER_PREFIX),avr-)
			CROSS_COMPILER_PATH   = /usr/local/bin
		else
			CROSS_COMPILER_PATH   = /usr/bin
		endif
		endif
	endif
    
endif

CROSS_COMPILER_PREFIX ?=
CROSS_COMPILER_SUFFIX ?=

CC      = $(CROSS_COMPILER_PATH)/$(CROSS_COMPILER_PREFIX)gcc$(CROSS_COMPILER_SUFFIX)
CC_SIZE = $(CROSS_COMPILER_PATH)/$(CROSS_COMPILER_PREFIX)size$(CROSS_COMPILER_SUFFIX)
CC_OBJ  = $(CROSS_COMPILER_PATH)/$(CROSS_COMPILER_PREFIX)objcopy$(CROSS_COMPILER_SUFFIX)
CC_LD   = $(CROSS_COMPILER_PATH)/$(CROSS_COMPILER_PREFIX)ld$(CROSS_COMPILER_SUFFIX)
CC_DUMP = $(CROSS_COMPILER_PATH)/$(CROSS_COMPILER_PREFIX)objdump$(CROSS_COMPILER_SUFFIX)
CC_COPY = $(CROSS_COMPILER_PATH)/$(CROSS_COMPILER_PREFIX)objcopy$(CROSS_COMPILER_SUFFIX)


RM			:= rm
MK			:= mkdir -p
CP			:= cp
MAKE_EXE		:= chmod ug=+rwx
MAKE_FILE_RIGHTS	:= chmod ug=rw
ECHO			:= echo

MAKE_FILE_RIGHTS	:= find ./$(APP_PATH) -type f -exec chmod ug+=rw {} \;
MAKE_FOLDER_RIGHTS	:= find ./$(APP_PATH) -type d -exec chmod ug+rwx {} \;

RM_FLAGS		:= -rf
VERBOSE 		:= @