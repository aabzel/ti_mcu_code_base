TEST_TBFP_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_tbfp_proto
#@echo $(error TEST_TBFP_DIR= $(TEST_TBFP_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR			+= -I$(TEST_TBFP_DIR)

SOURCES_C += $(TEST_TBFP_DIR)/test_tbfp_proto.c