PROTOCOLS_DIR = $(PROJECT_ROOT)protocols
#@echo $(error UTILS_DIR= $(UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(PROTOCOLS_DIR)

include $(PROTOCOLS_DIR)/rtcm3_protocol/rtcm3_protocol.mk


