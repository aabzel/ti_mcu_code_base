TEST_GNSS_UTILS_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_gnss_utils
#@echo $(error TEST_GNSS_UTILS_DIR= $(TEST_GNSS_UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(TEST_GNSS_UTILS_DIR)

SOURCES_C += $(TEST_GNSS_UTILS_DIR)/test_gnss_utils.c