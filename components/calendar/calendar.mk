CALENDAR_DIR = $(PROJECT_ROOT)components/calendar
#@ echo $(error CALENDAR_DIR = $(CALENDAR_DIR))

INCDIR += -I$(CALENDAR_DIR)

SOURCES_C += $(CALENDAR_DIR)/calendar.c
