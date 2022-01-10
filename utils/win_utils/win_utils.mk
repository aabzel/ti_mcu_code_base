WIN_UTILS_DIR = $(PROJECT_ROOT)utils/win_utils
#@echo $(error WIN_UTILS_DIR= $(WIN_UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))
#@echo $(error WIN_UTILS_DIR=$(WIN_UTILS_DIR))
INCDIR += -I$(WIN_UTILS_DIR)
OPT += -DWIN_UTILS

ifeq ($(HAS_SERIAL_PORT),Y)
    OPT += -DDEBUG_SERIAL
    SOURCES_C += $(WIN_UTILS_DIR)/cli_wrappers.c
    SOURCES_C += $(WIN_UTILS_DIR)/scan_serial_port.c
endif
SOURCES_C += $(WIN_UTILS_DIR)/win_utils.c
