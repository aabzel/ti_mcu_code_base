TEST_SET_SW_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw
#@echo $(error TEST_SET_SW_DIR= $(TEST_SET_SW_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR			+= -I$(TEST_SET_SW_DIR)

#include $(UNIT_TESTS_DIR)/string_utils/string_utils.mk


SOURCES_C += $(TEST_SET_SW_DIR)/test_sw_list.c

