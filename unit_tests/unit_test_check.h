#ifndef UNIT_TEST_CHECK_H
#define UNIT_TEST_CHECK_H

#include <math.h>
#include <string.h>

#include "diag_sys.h"
#include "io_utils.h"
#include "log.h"
#include "unit_test_run.h"

#define EXPECT_EQ_MEM(memL, memR, len)                                                                                 \
    do {                                                                                                               \
        if(0 != memcmp(memL, memR, len)) {                                                                             \
            LOG_ERROR(SYS, "Error");                                                                                   \
            io_printf("\n[e] %s:Line: %d diffenent memory", __FUNCTION__, __LINE__);                                   \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_NEAR(base, val, error)                                                                                  \
    do {                                                                                                               \
        if((base + error) < val) {                                                                                     \
            LOG_ERROR(SYS, "Error");                                                                                   \
            io_printf("\n[e] %s:Line: %d in val %f big ", __FUNCTION__, __LINE__, val);                                \
            return false;                                                                                              \
        }                                                                                                              \
        if(val < (base - error)) {                                                                                     \
            LOG_ERROR(SYS, "Error");                                                                                   \
            io_printf("\n[e] %s:Line: %d in val %f small ", __FUNCTION__, __LINE__, val);                              \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_TRUE(val_in)                                                                                            \
    do {                                                                                                               \
        bool val = val_in;                                                                                             \
        if(true != ((bool)val)) {                                                                                      \
            LOG_ERROR(SYS, "Error");                                                                                   \
            io_printf("\n[e] %s:Line: %d in val %d ", __FUNCTION__, __LINE__, (int)val);                               \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_FALSE(val_in)                                                                                           \
    do {                                                                                                               \
        bool val = val_in;                                                                                             \
        if(false != ((bool)val)) {                                                                                     \
            LOG_ERROR(SYS, "Error");                                                                                   \
            io_printf("\n[e] %s:Line: %d in val %d ", __FUNCTION__, __LINE__, (int)val);                               \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_EQ(val1, val2)                                                                                          \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if(val1 != val2) {                                                                                             \
            LOG_ERROR(SYS, "Error");                                                                                   \
            io_printf("\n[e] %s():Line: %d in val1: %u val2: %u ", __FUNCTION__, __LINE__, val1, val2);                \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#define EXPECT_STREQ(strL, strR)                                                                                       \
    do {                                                                                                               \
        if(0 != strcmp(strL, strR)) {                                                                                  \
            LOG_ERROR(SYS, "Error");                                                                                   \
            io_printf(CRLF "[e] %s:Line: %d in " CRLF "strL: [%s] " CRLF "strR: [%s] ", __FUNCTION__, __LINE__, strL,  \
                      strR);                                                                                           \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_BETWEEN(left, val, right)                                                                               \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if(val < left) {                                                                                               \
            LOG_ERROR(SYS, "Error");                                                                                   \
            io_printf("\n[e] %s():Line: %d in left: %u val: %u ", __FUNCTION__, __LINE__, left, val);                  \
            return false;                                                                                              \
        } else if(right < val) {                                                                                       \
            LOG_ERROR(SYS, "Error");                                                                                   \
            io_printf("\n[e] %s():Line: %d in val: %u right: %u ", __FUNCTION__, __LINE__, val, right);                \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#define EXPECT_EQ_SOFT(exp_v, val, precision)                                                                          \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if(val < (exp_v - precision)) {                                                                                \
            LOG_ERROR(SYS, "val too slow");                                                                            \
            io_printf("\n%s():Line: %d in val: %u exp_v: %u ", __FUNCTION__, __LINE__, val, exp_v);                    \
            return false;                                                                                              \
        } else if((exp_v + precision) < val) {                                                                         \
            LOG_ERROR(SYS, "val too big");                                                                             \
            io_printf("\n%s():Line: %d in val: %u exp_v: %u ", __FUNCTION__, __LINE__, val, exp_v);                    \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#endif /* UNIT_TEST_CHECK_H */
