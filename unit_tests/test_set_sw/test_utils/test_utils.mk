TEST_UTILS_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_utils
#@echo $(error TEST_UTILS_DIR= $(TEST_UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR			+= -I$(TEST_UTILS_DIR)



SOURCES_C += $(TEST_UTILS_DIR)/test_system.c
SOURCES_C += $(TEST_UTILS_DIR)/test_time_utils.c


SOURCES_C += $(TEST_UTILS_DIR)/bit_utils_test.c
SOURCES_C += $(TEST_UTILS_DIR)/byte_utils_test.c
SOURCES_C += $(TEST_UTILS_DIR)/data_utils_test.c
SOURCES_C += $(TEST_UTILS_DIR)/float_utils_test.c