TEST_SET_SW_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw
#@echo $(error TEST_SET_SW_DIR= $(TEST_SET_SW_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(TEST_SET_SW_DIR)

include $(TEST_SET_SW_DIR)/test_cli/test_cli.mk
include $(TEST_SET_SW_DIR)/test_crc/test_crc.mk
include $(TEST_SET_SW_DIR)/test_fifo/test_fifo.mk
include $(TEST_SET_SW_DIR)/test_rtcm3_proto/test_rtcm3_proto.mk
include $(TEST_SET_SW_DIR)/test_tbfp_proto/test_tbfp_proto.mk
include $(TEST_SET_SW_DIR)/test_nmea_proto/test_nmea_proto.mk
include $(TEST_SET_SW_DIR)/test_ublox_proto/test_ublox_proto.mk
include $(TEST_SET_SW_DIR)/test_string/test_string.mk
include $(TEST_SET_SW_DIR)/test_utils/test_utils.mk


SOURCES_C += $(TEST_SET_SW_DIR)/test_sw_list.c

