WIN_UTILS_DIR = $(PROJECT_ROOT)win_utils
#@echo $(error UTILS_DIR= $(UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))
INCDIR			+= -I$(WIN_UTILS_DIR)
OPT += -DWIN_UTILS
OPT += -DDEBUG_SERIAL

SOURCES += $(WIN_UTILS_DIR)/scan_serial_port.c
SOURCES += $(WIN_UTILS_DIR)/win_utils.c
