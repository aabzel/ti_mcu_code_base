GNSS_DIR = $(PROJECT_ROOT)components/gnss
#@ echo $(error GNSS_DIR = $(GNSS_DIR))

INCDIR += -I$(GNSS_DIR)

OPT += -DHAS_GNSS

SOURCES_C += $(GNSS_DIR)/gnss_utils.c
SOURCES_C += $(GNSS_DIR)/gnss_drv.c
SOURCES_C += $(GNSS_DIR)/gnss_diag.c