CODE_BASE_DIR = $(PROJECT_ROOT)
#@echo $(error CODE_BASE_DIR= $(CODE_BASE_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(CODE_BASE_DIR)

include $(CODE_BASE_DIR)/components/components.mk
include $(CODE_BASE_DIR)/bsp_cc26x2/bsp_cc26x2.mk
include $(CODE_BASE_DIR)/Drivers/drivers.mk
 


