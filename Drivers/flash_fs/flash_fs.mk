FLASH_FS_DIR = $(PROJECT_ROOT)Drivers/flash_fs
#@ echo $(error FLASH_FS_DIR = $(FLASH_FS_DIR))
#@ echo $(error CFLAGS = $(CFLAGS)) 

INCDIR += -I$(FLASH_FS_DIR)
