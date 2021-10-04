#ifndef UNIT_TEST_CHECK_X86_H
#define UNIT_TEST_CHECK_X86_H

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
//#include "io_utils.h"
//#include "unit_test_run.h"
/*Macros for Polymorphism*/
#define EXPECT_EQ_MEM(memL, memR, len)                                                                                 \
    do {                                                                                                               \
        int ret = memcmp(memL, memR, len);                                                                             \
        if(0 != ret) {                                                                                                 \
            printf(  "\n%s:Line: %d different memory %d\n", __FUNCTION__, __LINE__, ret);                            \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_NEAR(base, val, error)                                                                                  \
    do {                                                                                                               \
        if((base + error) < val) {                                                                                     \
            printf(  "\n%s:Line: %d in val %f big \n\n", __FUNCTION__, __LINE__, val);                                 \
            return false;                                                                                              \
        }                                                                                                              \
        if(val < (base - error)) {                                                                                     \
            printf(  "\n%s:Line: %d in val %f small \n\n", __FUNCTION__, __LINE__, val);                               \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_TRUE(val_in)                                                                                            \
    do {                                                                                                               \
        bool val = val_in;                                                                                             \
        if(true != ((bool)val)) {                                                                                      \
            printf(  "\n%s:Line: %d in val %d \n", __FUNCTION__, __LINE__, (int)val);                                \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_FALSE(val_in)                                                                                           \
    do {                                                                                                               \
        bool val = val_in;                                                                                             \
        if(false != ((bool)val)) {                                                                                     \
            printf(  "\n%s:Line: %d in val %d \n", __FUNCTION__, __LINE__, (int)val);                                \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_EQ_U64(val1, val2)                                                                                      \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if(val1 != val2) {                                                                                             \
            printf(  "\n%s():Line: %d in val1: %" PRIu64 " val2: %" PRIu64 "\n", __FUNCTION__, __LINE__, val1,       \
                      val2);                                                                                           \
            printf(  "\n%s():Line: %d in val1: 0x%" PRIx64 " val2: 0x%" PRIx64 "\n", __FUNCTION__, __LINE__, val1,   \
                      val2);                                                                                           \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#define EXPECT_EQ(val1, val2)                                                                                          \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if(val1 != val2) {                                                                                             \
            printf(  "\n%s():Line: %d in val1: %u  val2: %u\n", __FUNCTION__, __LINE__, val1, val2);                 \
            printf(  "\n%s():Line: %d in val1: 0x%x val2: 0x%x\n", __FUNCTION__, __LINE__, val1, val2);              \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#define EXPECT_EQ_VARS(var1, var2, val)                                                                                \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if(!((var1 == val) || (var2 == val))) {                                                                        \
            printf(  "\n%s():Line:%d in var1:%u var2:%u val:%u \n", __FUNCTION__, __LINE__, var1, var2, val);        \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#define EXPECT_NE(val1, val2)                                                                                          \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if((val1) == (val2)) {                                                                                         \
            printf(  "\n%s():Line: %d in val1: %u val2: %u \n", __FUNCTION__, __LINE__, val1, val2);                 \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#define EXPECT_STREQ(strL, strR)                                                                                       \
    do {                                                                                                               \
        if(0 != strcmp(strL, strR)) {                                                                                  \
            printf(  "\n%s:Line: %d in " CRLF "strL: [%s] " CRLF "strR: [%s] \n", __FUNCTION__, __LINE__, strL,      \
                      strR);                                                                                           \
            return false;                                                                                              \
        }                                                                                                              \
    } while(0);

#define EXPECT_BETWEEN(left, val, right)                                                                               \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if((val) < (left)) {                                                                                           \
            printf(  "\n%s():Line: %d in left: %u val: %u \n", __FUNCTION__, __LINE__, left, val);                   \
            return false;                                                                                              \
        } else if((right) < (val)) {                                                                                   \
            printf(  "\n%s():Line: %d in val: %u right: %u \n", __FUNCTION__, __LINE__, val, right);                 \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#define EXPECT_EQ_SOFT(exp_v, val, precision)                                                                          \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if((val) < (exp_v - precision)) {                                                                              \
            printf(  "\n%s():Line: %d in val: %u exp_v: %u \n", __FUNCTION__, __LINE__, val, exp_v);                 \
            return false;                                                                                              \
        } else if((exp_v + precision) < val) {                                                                         \
            printf(  "\n%s():Line: %d in val: %u exp_v: %u \n", __FUNCTION__, __LINE__, val, exp_v);                 \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#define EXPECT_GR(val1, val2, id)                                                                                      \
    do {                                                                                                               \
        /*int val1 = vala; */                                                                                          \
        /*int val2 = val2; */                                                                                          \
        if((val2) < (val1)) {                                                                                          \
            printf(  "\n%s():Line: %d in val1: %llu val2: %llu id: %u\n", __FUNCTION__, __LINE__, val1, val2, id);   \
            printf(  "\n%s():Line: %d in val1: 0x%llx val2: 0x%llx id: %u\n", __FUNCTION__, __LINE__, val1, val2,    \
                      id);                                                                                             \
            return false;                                                                                              \
        } else {                                                                                                       \
            /*printf ("\n OK!\n");  */                                                                                 \
        }                                                                                                              \
    } while(0);

#endif /* UNIT_TEST_CHECK_X86_H */
