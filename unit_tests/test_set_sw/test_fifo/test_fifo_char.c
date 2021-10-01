#include "test_fifo_char.h"


#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "fifo_char.h"
#include "unit_test_check.h"

Fifo_array_t fifiObj;
char array[5];

#define STRING19 "123456789"

bool test_fifo_array1_char(void) {
    Fifo_array_t fifiObj1;
    char array1[6];
    fifo_init(&fifiObj,  array1,sizeof(array1));
    EXPECT_FALSE(fifo_push_array(&fifiObj1, STRING19, strlen(STRING19)));
    uint16_t outLen = 0;
    char outArray[100];
    EXPECT_TRUE(fifo_pull_array(&fifiObj1, outArray, &outLen));
    EXPECT_EQ(6, outLen);
    EXPECT_STREQ("123456", outArray);
    return true;
}

bool test_fifo_array_char(void) {
    Fifo_array_t fifiObj1;
    char array1[10];
    char outArray[100];
    uint16_t outLen = 0;
    fifo_init(&fifiObj,  array1,sizeof(array1));

    EXPECT_TRUE(fifo_push_array(&fifiObj1, "12345", 5));
    EXPECT_TRUE(fifo_pull_array(&fifiObj1, outArray, &outLen));

    EXPECT_EQ(5, outLen);

    EXPECT_STREQ("12345", outArray);

    return true;
}

bool test_fifo_char(void) {

    char outChar = '0';
    fifo_init(&fifiObj,  array,sizeof(array));

    EXPECT_TRUE(fifo_push(&fifiObj, '1'));
    EXPECT_TRUE(fifo_push(&fifiObj, '2'));
    EXPECT_TRUE(fifo_push(&fifiObj, '3'));
    EXPECT_TRUE(fifo_push(&fifiObj, '4'));
    EXPECT_TRUE(fifo_push(&fifiObj, '5'));
    EXPECT_FALSE(fifo_push(&fifiObj, '6'));

    EXPECT_TRUE(fifo_pull(&fifiObj, &outChar));
    EXPECT_EQ('1', outChar);

    EXPECT_TRUE(fifo_pull(&fifiObj, &outChar));
    EXPECT_EQ('2', outChar);

    EXPECT_TRUE(fifo_pull(&fifiObj, &outChar));
    EXPECT_EQ('3', outChar);

    EXPECT_TRUE(fifo_pull(&fifiObj, &outChar));
    EXPECT_EQ('4', outChar);

    EXPECT_TRUE(fifo_pull(&fifiObj, &outChar));
    EXPECT_EQ('5', outChar);

    EXPECT_FALSE(fifo_pull(&fifiObj, &outChar));
    EXPECT_FALSE(test_fifo_array_char());

    EXPECT_FALSE(test_fifo_array1_char());

    return true;
}
