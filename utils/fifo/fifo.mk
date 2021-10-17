FIFO_DIR = $(PROJECT_ROOT)utils/fifo
#@echo $(error FIFO_DIR= $(FIFO_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(FIFO_DIR)

SOURCES_C += $(FIFO_DIR)/fifo_array.c
SOURCES_C += $(FIFO_DIR)/fifo_char.c
SOURCES_C += $(FIFO_DIR)/fifo_index.c

