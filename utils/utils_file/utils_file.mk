UTILS_FILE_DIR = $(PROJECT_ROOT)utils/utils_file
#@echo $(error STRING_UTILS_DIR=$(STRING_UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))
INCDIR			+= -I$(UTILS_FILE_DIR)
OPT += -DHAS_UTILS_FILE

SOURCES += $(UTILS_FILE_DIR)/utils_file.c

