UTILS_DIR = $(PROJECT_ROOT)utils
#@echo $(error UTILS_DIR= $(UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(UTILS_DIR)

include $(UTILS_DIR)/utils_string/utils_string.mk
include $(UTILS_DIR)/crc/crc.mk

SOURCES_C += $(UTILS_DIR)/array.c
SOURCES_C += $(UTILS_DIR)/time_utils.c
SOURCES_C += $(UTILS_DIR)/bit_utils.c
SOURCES_C += $(UTILS_DIR)/byte_utils.c
SOURCES_C += $(UTILS_DIR)/data_utils.c
SOURCES_C += $(UTILS_DIR)/fifo_array.c
SOURCES_C += $(UTILS_DIR)/fifo_char.c
SOURCES_C += $(UTILS_DIR)/fifo_index.c
SOURCES_C += $(UTILS_DIR)/float_utils.c
#SOURCES_C += $(UTILS_DIR)/utils.c

