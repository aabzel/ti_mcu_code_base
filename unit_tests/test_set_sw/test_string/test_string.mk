TEST_STRING_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_string
#@echo $(error TEST_STRING_DIR= $(TEST_STRING_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(TEST_STRING_DIR)

SOURCES_C += $(TEST_STRING_DIR)/text_convert.c
SOURCES_C += $(TEST_STRING_DIR)/test_string_utils.c