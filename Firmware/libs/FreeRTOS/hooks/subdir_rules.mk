
FR_H_PATH := $(FR_PATH)/hooks
FR_H_PATH_OUT := $(FR_PATH_OUT)/hooks

OBJECTS += $(patsubst $(FR_H_PATH)/%.c, $(FR_H_PATH_OUT)/%.o, \
		   $(wildcard $(FR_H_PATH)/*.c) )

$(FR_H_PATH_OUT)/%.o : $(FR_H_PATH)/%.c
	@echo ===========================================
	@echo Generating file $@
	@mkdir -p $(FR_H_PATH_OUT)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@
