
FR_T_PATH := $(FR_PATH)/tools
FR_T_PATH_OUT := $(FR_PATH_OUT)/tools

OBJECTS += $(patsubst $(FR_T_PATH)/%.c, $(FR_T_PATH_OUT)/%.o, \
		   $(wildcard $(FR_T_PATH)/*.c) )

$(FR_T_PATH_OUT)/%.o : $(FR_T_PATH)/%.c
	@echo ===========================================
	@echo Generating file $@
	@mkdir -p $(FR_T_PATH_OUT)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@
