RTCM3_PROTOCOL_DIR = $(PROJECT_ROOT)protocols/rtcm3_protocol
#@ echo $(error RTCM3_PROTOCOL_DIR = $(RTCM3_PROTOCOL_DIR)) 
#@ echo $(error CFLAGS = $(CFLAGS))

INCDIR += -I$(RTCM3_PROTOCOL_DIR)

SOURCES_C +=$(RTCM3_PROTOCOL_DIR)/rtcm3_protocol.c
