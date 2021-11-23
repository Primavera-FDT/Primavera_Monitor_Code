# User Config Options
# =============================================================================

PROGRAM = program

DEVICE ?= MSP432P401R

OBJ_DIR := output
LIB_DIR := libs
FREERTOS_DIR := $(LIB_DIR)/FreeRTOS
FREERTOS_LIB := libFreeRTOS.a

GCC_MSP_INC_DIR := include/msp_headers
GCC_CMSIS_INC_DIR := $(GCC_MSP_INC_DIR)/CMSIS
LD_SCRIPT := $(GCC_MSP_INC_DIR)/$(shell echo $(DEVICE) | tr A-Z a-z)

GCC_INC_DIR := /usr/lib/gcc/arm-none-eabi/include
CC := arm-none-eabi-gcc
GDB := arm-none-eabi-gdb
OBJCOPY := arm-none-eabi-objcopy

FLASHER := dslite.sh
CONFIG_FILE := MSP432P401R.ccxml

# System config options
# =============================================================================

INCLUDES := -I $(GCC_CMSIS_INC_DIR) -I $(GCC_MSP_INC_DIR) -I $(GCC_INC_DIR) \
			-I include/ -I $(FREERTOS_DIR)/include -I $(FREERTOS_DIR)/hooks \
			-I $(FREERTOS_DIR)/portable

DEVICE_FLAGS := -mcpu=cortex-m4 -march=armv7e-m -mfloat-abi=hard \
				-mfpu=fpv4-sp-d16 -mthumb -D__$(DEVICE)__ \
				-DTARGET_IS_MSP432P4XX

GCC_FLAGS := -Dgcc -g -gstrict-dwarf -Wall

CFLAGS := -c $(DEVICE_FLAGS) $(GCC_FLAGS) -ffunction-sections \
		  -fdata-sections -MD -std=c99

LDFLAGS := $(DEVICE_FLAGS) $(GCC_FLAGS) -T$(LD_SCRIPT).lds -l'c' -l'gcc' \
		   -l'rdimon' --specs=nosys.specs

# Objects
# =============================================================================

OBJECTS=$(patsubst src/%.c, $(OBJ_DIR)/%.o, $(wildcard src/*.c))

# Includes
# =============================================================================

include $(FREERTOS_DIR)/subdir_rules.mk

# Rules
# =============================================================================

.PHONY: clean all upload

all : $(OBJ_DIR)/$(PROGRAM).hex

$(OBJ_DIR)/$(PROGRAM).hex : $(OBJ_DIR)/$(PROGRAM).out
	@echo =========================================
	@echo Creating Flash image
	$(OBJCOPY) $^ $@

$(OBJ_DIR)/$(PROGRAM).out : $(OBJECTS)
	@echo =========================================
	@echo Linking object files
	$(CC) $(LDFLAGS) $^ -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : src/%.c
	@echo =========================================
	@echo Generating file $@
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

clean:
	rm -rf $(OBJ_DIR)

upload:
	$(FLASHER) -f --config=$(CONFIG_FILE) -u $(OBJ_DIR)/$(PROGRAM).hex
