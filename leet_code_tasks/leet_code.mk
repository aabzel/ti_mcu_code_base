LEET_CODE_DIR = $(PROJECT_ROOT)leet_code_tasks
#@echo $(error LEET_CODE_DIR= $(LEET_CODE_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(LEET_CODE_DIR)
 
SOURCES_C += $(LEET_CODE_DIR)/implement_queue.c


