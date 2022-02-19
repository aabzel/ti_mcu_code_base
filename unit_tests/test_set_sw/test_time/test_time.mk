TEST_TIME_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_time
#@echo $(error TEST_TIME_DIR= $(TEST_TIME_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(TEST_TIME_DIR)

SOURCES_C += $(TEST_TIME_DIR)/test_time_utils.c
