UTILS_FILE_DIR = $(PROJECT_ROOT)utils/utils_files
#@echo $(error STRING_UTILS_DIR=$(STRING_UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))
#@echo $(error UTILS_FILE_DIR= $(UTILS_FILE_DIR))

INCDIR += -I$(UTILS_FILE_DIR)
OPT += -DHAS_UTILS_FILE

SOURCES_C += $(UTILS_FILE_DIR)/utils_file.c

