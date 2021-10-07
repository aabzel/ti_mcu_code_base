#include "test_fifo_char.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fifo_char.h"
#include "unit_test_check.h"

#define STRING19 "123456789"

bool test_fifo_char_array_overrun(void) {
    Fifo_array_t FifiObj;
    char peek_ch = '0';
    char FiFoHeap[6] = "";
    fifo_init(&FifiObj, FiFoHeap, sizeof(FiFoHeap));
    EXPECT_FALSE(fifo_push_array(&FifiObj, STRING19, strlen(STRING19)));

    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('1', peek_ch);

    uint16_t outLen = 0;
    char outArray[100] = "";
    EXPECT_TRUE(fifo_pull_array(&FifiObj, outArray, &outLen));
    EXPECT_EQ(6, outLen);
    EXPECT_STREQ("123456", outArray);
    return true;
}

bool test_fifo_char_array(void) {
    Fifo_array_t FifiObj;
    char FiFoHeap[10] = "";
    char outArray[100] = "";
    char peek_ch = '0';
    uint16_t outLen = 0;
    fifo_init(&FifiObj, FiFoHeap, sizeof(FiFoHeap));

    EXPECT_TRUE(fifo_push_array(&FifiObj, "12345", 5));

    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('1', peek_ch);

    EXPECT_TRUE(fifo_pull_array(&FifiObj, outArray, &outLen));

    EXPECT_EQ(5, outLen);

    EXPECT_STREQ("12345", outArray);

    return true;
}

bool test_fifo_char_overrun(void) {
    Fifo_array_t FifiObj;
    char FiFoHeap[5] = "";

    char outChar = '0';
    char peek_ch = '0';
    fifo_init(&FifiObj, FiFoHeap, sizeof(FiFoHeap));

    EXPECT_TRUE(fifo_push(&FifiObj, '1'));
    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('1', peek_ch);
    EXPECT_TRUE(fifo_push(&FifiObj, '2'));
    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('1', peek_ch);
    EXPECT_TRUE(fifo_push(&FifiObj, '3'));
    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('1', peek_ch);
    EXPECT_TRUE(fifo_push(&FifiObj, '4'));
    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('1', peek_ch);
    EXPECT_TRUE(fifo_push(&FifiObj, '5'));
    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_FALSE(fifo_push(&FifiObj, '6'));

    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('1', peek_ch);
    EXPECT_TRUE(fifo_pull(&FifiObj, &outChar));
    EXPECT_EQ('1', outChar);

    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('2', peek_ch);
    EXPECT_TRUE(fifo_pull(&FifiObj, &outChar));
    EXPECT_EQ('2', outChar);

    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('3', peek_ch);
    EXPECT_TRUE(fifo_pull(&FifiObj, &outChar));
    EXPECT_EQ('3', outChar);

    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('4', peek_ch);
    EXPECT_TRUE(fifo_pull(&FifiObj, &outChar));
    EXPECT_EQ('4', outChar);

    EXPECT_TRUE(fifo_peek(&FifiObj, &peek_ch));
    EXPECT_EQ('5', peek_ch);
    EXPECT_TRUE(fifo_pull(&FifiObj, &outChar));
    EXPECT_EQ('5', outChar);

    EXPECT_FALSE(fifo_pull(&FifiObj, &outChar));

    return true;
}
