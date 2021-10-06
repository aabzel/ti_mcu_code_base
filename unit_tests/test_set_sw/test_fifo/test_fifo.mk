TEST_FIFO_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_fifo
#@echo $(error TEST_FIFO_DIR= $(TEST_FIFO_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR			+= -I$(TEST_FIFO_DIR)

SOURCES_C += $(TEST_FIFO_DIR)/test_fifo_array.c
SOURCES_C += $(TEST_FIFO_DIR)/test_fifo_char.c
SOURCES_C += $(TEST_FIFO_DIR)/test_fifo_indexer.c
