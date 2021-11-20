
FR_P_PATH := $(FR_PATH)/portable
FR_P_PATH_OUT := $(FR_PATH_OUT)/portable

OBJECTS += $(patsubst $(FR_P_PATH)/%.c, $(FR_P_PATH_OUT)/%.o, \
		   $(wildcard $(FR_P_PATH)/*.c) )

$(FR_P_PATH_OUT)/%.o : $(FR_P_PATH)/%.c
	@echo ===========================================
	@echo Generating file $@
	@mkdir -p $(FR_P_PATH_OUT)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@
