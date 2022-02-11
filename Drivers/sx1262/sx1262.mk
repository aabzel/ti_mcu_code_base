SX1262_DIR = $(PROJECT_ROOT)Drivers/sx1262
#@ echo $(error SX1262_DIR = $(SX1262_DIR))
#@ echo $(error CFLAGS = $(CFLAGS)) 

INCDIR += -I$(SX1262_DIR)
