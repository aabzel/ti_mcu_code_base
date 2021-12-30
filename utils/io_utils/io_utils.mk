IO_UTILS_DIR = $(PROJECT_ROOT)utils/io_utils
#@echo $(error STRING_UTILS_DIR=$(STRING_UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))
INCDIR			+= -I$(IO_UTILS_DIR)
#OPT += -Dio_printf=printf
SOURCES_C += $(IO_UTILS_DIR)/log.c
#SOURCES_C += $(IO_UTILS_DIR)/.c
