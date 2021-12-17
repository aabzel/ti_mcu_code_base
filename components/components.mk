COMPONENTS_DIR = $(PROJECT_ROOT)components
#@echo $(error COMPONENTS_DIR= $(COMPONENTS_DIR))

INCDIR += -I$(COMPONENTS_DIR)
include $(COMPONENTS_DIR)/calendar/calendar.mk
include $(COMPONENTS_DIR)/system/system.mk

