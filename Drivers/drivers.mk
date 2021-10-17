DRIVERS_DIR = $(PROJECT_ROOT)Drivers
#@echo $(error DRIVERS_DIR= $(DRIVERS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(DRIVERS_DIR)

include $(DRIVERS_DIR)/cli_drv/cli_drv.mk


