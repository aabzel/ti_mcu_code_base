CLI_DRV_DIR = $(PROJECT_ROOT)Drivers/cli_drv
#@echo $(error CLI_DRV_DIR= $(CLI_DRV_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(CLI_DRV_DIR)

SOURCES_C +=  $(CLI_DRV_DIR)/cli_manager.c


