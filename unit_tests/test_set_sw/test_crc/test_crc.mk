TEST_CRC_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_crc
#@echo $(error TEST_CRC_DIR= $(TEST_CRC_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR  += -I$(TEST_CRC_DIR)

SOURCES_C += $(TEST_CRC_DIR)/test_crc.c

