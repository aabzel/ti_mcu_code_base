WIN_UTILS_DIR = $(PROJECT_ROOT)utils/win_utils
#@echo $(error UTILS_DIR= $(UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))
#@echo $(error WIN_UTILS_DIR=$(WIN_UTILS_DIR))
INCDIR			+= -I$(WIN_UTILS_DIR)
OPT += -DWIN_UTILS
OPT += -DDEBUG_SERIAL

SOURCES_C += $(WIN_UTILS_DIR)/cli_wrappers.c
SOURCES_C += $(WIN_UTILS_DIR)/scan_serial_port.c
SOURCES_C += $(WIN_UTILS_DIR)/win_utils.c
