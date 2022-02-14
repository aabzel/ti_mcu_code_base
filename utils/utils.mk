UTILS_DIR = $(PROJECT_ROOT)utils
#@echo $(error UTILS_DIR= $(UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(UTILS_DIR)

include $(UTILS_DIR)/io_utils/io_utils.mk
include $(UTILS_DIR)/utils_string/utils_string.mk
include $(UTILS_DIR)/crc/crc.mk
include $(UTILS_DIR)/fifo/fifo.mk
include $(UTILS_DIR)/win_utils/win_utils.mk

SOURCES_C += $(UTILS_DIR)/time_diag.c
SOURCES_C += $(UTILS_DIR)/array.c
SOURCES_C += $(UTILS_DIR)/bit_utils.c
SOURCES_C += $(UTILS_DIR)/byte_utils.c
SOURCES_C += $(UTILS_DIR)/data_utils.c
SOURCES_C += $(UTILS_DIR)/debug_info.c
SOURCES_C += $(UTILS_DIR)/float_utils.c
SOURCES_C += $(UTILS_DIR)/time_utils.c
#SOURCES_C += $(UTILS_DIR)/utils.c

