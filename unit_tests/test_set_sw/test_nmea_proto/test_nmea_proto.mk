TEST_NMEA_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_nmea_proto
#@echo $(error TEST_NMEA_DIR= $(TEST_NMEA_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR			+= -I$(TEST_NMEA_DIR)

SOURCES_C += $(TEST_NMEA_DIR)/test_nmea_proto.c
SOURCES_C += $(TEST_NMEA_DIR)/test_gnss_utils.c