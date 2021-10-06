COMMON_DIR = $(PROJECT_ROOT)common
#@echo $(error COMMON_DIR= $(COMMON_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(COMMON_DIR)

 
SOURCES_C += $(COMMON_DIR)/uart_common.c

