CRC_DIR = $(PROJECT_ROOT)utils/crc
#@echo $(error CRC_DIR=$(CRC_DIR))
#@echo $(error CFLAGS= $(CFLAGS))
INCDIR  += -I$(CRC_DIR)

SOURCES_C += $(CRC_DIR)/crc24_q.c
#SOURCES_C += $(CRC_DIR)/.c
