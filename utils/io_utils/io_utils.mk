IO_UTILS_DIR = $(PROJECT_ROOT)utils/io_utils
#@echo $(error IO_UTILS_DIR=$(IO_UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))
INCDIR += -I$(IO_UTILS_DIR)
#OPT += -Dio_printf=printf
SOURCES_C += $(IO_UTILS_DIR)/log.c
SOURCES_C += $(IO_UTILS_DIR)/writer_config.c
SOURCES_C += $(IO_UTILS_DIR)/writer_generic.c
SOURCES_C += $(IO_UTILS_DIR)/writer_stdout.c
SOURCES_C += $(IO_UTILS_DIR)/ostream.c


