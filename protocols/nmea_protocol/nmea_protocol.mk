NMEA_PROTO_DIR = $(PROJECT_ROOT) protocols /
                 nmea_protocol #@ echo $(error NMEA_PROTO_DIR = $(NMEA_PROTO_DIR)) #@ echo $(error CFLAGS = $(CFLAGS))

                     INCDIR += -I$(NMEA_PROTO_DIR)

                                   SOURCES_C += $(NMEA_PROTO_DIR) / nmea_protocol.c
