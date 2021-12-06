BSP_CC26X2_DIR = $(PROJECT_ROOT)bsp_cc26x2
#@echo $(error BSP_CC26X2_DIR= $(BSP_CC26X2_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(BSP_CC26X2_DIR)

include $(BSP_CC26X2_DIR)/flash/flash.mk



