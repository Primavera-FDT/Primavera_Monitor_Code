
FR_PATH := $(FREERTOS_DIR)
FR_PATH_OUT := $(OBJ_DIR)/$(shell basename $(FREERTOS_DIR) )

OBJECTS += $(patsubst $(FR_PATH)/%.c, $(FR_PATH_OUT)/%.o, \
		   $(wildcard $(FR_PATH)/*.c) )

include $(FREERTOS_DIR)/hooks/subdir_rules.mk
include $(FREERTOS_DIR)/portable/subdir_rules.mk
include $(FREERTOS_DIR)/tools/subdir_rules.mk

$(FR_PATH_OUT)/%.o : $(FR_PATH)/%.c
	@echo ===========================================
	@echo Generating file $@
	@mkdir -p $(FR_PATH_OUT)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@
