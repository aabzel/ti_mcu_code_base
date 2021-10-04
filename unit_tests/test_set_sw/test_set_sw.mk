TEST_SET_SW_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw
#@echo $(error TEST_SET_SW_DIR= $(TEST_SET_SW_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR			+= -I$(TEST_SET_SW_DIR)

#include $(TEST_SET_SW_DIR)/string_utils/string_utils.mk
include $(TEST_SET_SW_DIR)/test_utils/test_utils.mk
include $(TEST_SET_SW_DIR)/test_rtcm3_proto/test_rtcm3_proto.mk

#SOURCES_C += $(TEST_SET_SW_DIR)/test_sw_list.c

