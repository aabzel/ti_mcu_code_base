UTILS_DIR = $(PROJECT_ROOT)utils
#@echo $(error UTILS_DIR= $(UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR			+= -I$(UTILS_DIR)

#include $(UTILS_DIR)/string_utils/string_utils.mk

#SOURCES += $(UTILS_DIR)/bit_utils.c
#SOURCES += $(UTILS_DIR)/byte_utils.c
SOURCES += $(UTILS_DIR)/float_utils.c
#SOURCES += $(UTILS_DIR)/utils.c

