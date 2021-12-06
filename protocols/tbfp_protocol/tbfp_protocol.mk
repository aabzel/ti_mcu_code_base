TBFP_PROTOCOL_DIR = $(PROJECT_ROOT)protocols/tbfp_protocol 
#@ echo $(error TBFP_PROTOCOL_DIR = $(TBFP_PROTOCOL_DIR)) 
#@ echo $(error CFLAGS = $(CFLAGS))

INCDIR += -I$(TBFP_PROTOCOL_DIR)

SOURCES_C +=$(TBFP_PROTOCOL_DIR)/tbfp_protocol.c 
SOURCES_C +=$(TBFP_PROTOCOL_DIR)/tbfp_protocol_parser.c
