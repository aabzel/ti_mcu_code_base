TEST_RTCM3_DIR =
    $(PROJECT_ROOT) unit_tests / test_set_sw /
    test_rtcm3_proto #@ echo $(error TEST_RTCM3_DIR = $(TEST_RTCM3_DIR)) #@ echo $(error CFLAGS = $(CFLAGS))

        INCDIR += -I$(TEST_RTCM3_DIR)

                      SOURCES_C += $(TEST_RTCM3_DIR) / test_rtcm3_proto.c