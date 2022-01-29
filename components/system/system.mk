SYSTEM_DIR = $(PROJECT_ROOT)components/system
#@ echo $(error SYSTEM_DIR = $(SYSTEM_DIR))

INCDIR += -I$(SYSTEM_DIR)

SOURCES_C += $(SYSTEM_DIR)/system.c