TEST_TBFP_RTCM_DIR = $(PROJECT_ROOT)unit_tests/test_set_sw/test_tbfp_rtcm_proto
#@echo $(error TEST_TBFP_RTCM_DIR= $(TEST_TBFP_RTCM_DIR))
#@echo $(error CFLAGS= $(CFLAGS))

INCDIR += -I$(TEST_TBFP_RTCM_DIR)

SOURCES_C += $(TEST_TBFP_RTCM_DIR)/test_tbfp_rtcm_proto.c