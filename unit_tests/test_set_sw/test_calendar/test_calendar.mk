TEST_CALENDAR_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_calendar
#@echo $(error TEST_CALENDAR_DIR= $(TEST_CALENDAR_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(TEST_CALENDAR_DIR)

SOURCES_C += $(TEST_CALENDAR_DIR)/test_calendar.c
