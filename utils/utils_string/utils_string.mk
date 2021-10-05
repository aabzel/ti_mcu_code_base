STRING_UTILS_DIR = $(PROJECT_ROOT)utils/utils_string
#@echo $(error PROJECT_ROOT=$(PROJECT_ROOT))
#@echo $(error STRING_UTILS_DIR=$(STRING_UTILS_DIR))
#@echo $(error CFLAGS= $(CFLAGS))
INCDIR += -I$(STRING_UTILS_DIR)

SOURCES_C += $(STRING_UTILS_DIR)/convert.c
SOURCES_C += $(STRING_UTILS_DIR)/str_utils.c

