UNIT_TESTS_DIR = $(PROJECT_ROOT)unit_tests
#@echo $(error UNIT_TESTS_DIR= $(UNIT_TESTS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR			+= -I$(UNIT_TESTS_DIR)

include $(UNIT_TESTS_DIR)/test_set_sw/test_set_sw.mk

#SOURCES_C += $(UNIT_TESTS_DIR)/bit_utils.c
#SOURCES_C += $(UNIT_TESTS_DIR)/print_buffer.c
#SOURCES_C += $(UNIT_TESTS_DIR)/unit_test_run.c
SOURCES_C += $(UNIT_TESTS_DIR)/test_list.c

