TEST_CLI_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_cli
#@echo $(error TEST_UTILS_DIR= $(TEST_UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(TEST_CLI_DIR)

SOURCES_C += $(TEST_CLI_DIR)/test_cli.c

