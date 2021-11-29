#ifndef UNIT_TEST_MACRO_H
#define UNIT_TEST_MACRO_H
#if 0
#include "unit_test_info.h"

#define PRAGMA(x) _Pragma(#x)

#ifdef __cplusplus
    #define TEST(suite, test_case)                                                                                    \
        extern "C" {                                                                                                  \
        static void(ut_##suite##_##test_case)(void);                                                                  \
        extern const unit_test_info_t _ut_##suite##_##test_case;                                                      \
        PRAGMA(ghs reference _ut_##suite##_##test_case)                                                               \
        __attribute__((used)) __attribute__((section(".unit_test_info")))                                             \
            const unit_test_info_t _ut_##suite##_##test_case = {(#suite "." #test_case), (ut_##suite##_##test_case)}; \
        }                                                                                                             \
        __attribute__((used)) static void(ut_##suite##_##test_case)(void)
#else
    #define TEST(suite, test_case)                                                                                    \
        extern const unit_test_info_t _ut_##suite##_##test_case;                                                      \
        static void(ut_##suite##_##test_case)(void);                                                                  \
        PRAGMA(ghs reference _ut_##suite##_##test_case)                                                               \
        __attribute__((used)) __attribute__((section(".unit_test_info")))                                             \
            const unit_test_info_t _ut_##suite##_##test_case = {(#suite "." #test_case), (ut_##suite##_##test_case)}; \
        static void(ut_##suite##_##test_case)(void)
#endif
#endif
#endif /* UNIT_TEST_MACRO_H */
