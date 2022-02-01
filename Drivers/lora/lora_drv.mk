LORA_DRV_DIR = $(PROJECT_ROOT)Drivers/lora 
#@ echo $(error LORA_DRV_DIR = $(LORA_DRV_DIR)) 
#@ echo $(error CFLAGS = $(CFLAGS)) 

INCDIR += -I$(LORA_DRV_DIR)

SOURCES_C += $(LORA_DRV_DIR)/lora_drv.c

