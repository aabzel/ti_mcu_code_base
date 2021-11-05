#include "test_fifo_array.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "data_utils.h"
#include "fifo_array.h"
#include "unit_test_check.h"

uint8_t arr5[5]={1,2,3,4,5};
uint8_t arr3[3]={1,2,3};
uint8_t arr2[2]={1,2};
bool test_fifo_array(void) {
#ifdef X86_64
    printf("\n%s():", __FUNCTION__);
#endif
    FifoArray_t FiFoArr;
    Array_t NodeHeap[5];
    EXPECT_TRUE( fifo_arr_init(&FiFoArr, &NodeHeap[0], ARRAY_SIZE(NodeHeap)));
    EXPECT_EQ(5, fifo_arr_get_size( &FiFoArr));
    EXPECT_EQ(0, fifo_arr_get_count( &FiFoArr));

    Array_t rdNode;
    Array_t peekNode;
    EXPECT_FALSE( fifo_arr_peek(&FiFoArr, &peekNode));
    EXPECT_FALSE( fifo_arr_pull(&FiFoArr, &rdNode));

    Array_t wrNode;
    wrNode.pArr=arr2;
    wrNode.size=2;
    EXPECT_TRUE( fifo_arr_push(&FiFoArr, wrNode));
    EXPECT_TRUE( fifo_arr_peek(&FiFoArr, &peekNode));
    EXPECT_EQ(2,peekNode.size);

    wrNode.pArr=arr3;
    wrNode.size=3;
    EXPECT_TRUE( fifo_arr_push(&FiFoArr, wrNode));
    EXPECT_TRUE( fifo_arr_peek(&FiFoArr, &peekNode));
    EXPECT_EQ(2,peekNode.size);

    wrNode.pArr=arr5;
    wrNode.size=5;
    EXPECT_TRUE( fifo_arr_push(&FiFoArr, wrNode));
    EXPECT_TRUE( fifo_arr_peek(&FiFoArr, &peekNode));
    EXPECT_EQ(2,peekNode.size);

    EXPECT_EQ(3, fifo_arr_get_count( &FiFoArr));

    EXPECT_TRUE(  fifo_arr_pull(&FiFoArr, &rdNode));
    EXPECT_EQ(2,rdNode.size);
    EXPECT_EQ_MEM(arr2, rdNode.pArr, 2);

    EXPECT_TRUE( fifo_arr_peek(&FiFoArr, &peekNode));
    EXPECT_EQ(3,peekNode.size);
    EXPECT_TRUE(  fifo_arr_pull(&FiFoArr, &rdNode));
    EXPECT_EQ(3,rdNode.size);
    EXPECT_EQ_MEM(arr3, rdNode.pArr, 3);

    EXPECT_TRUE( fifo_arr_peek(&FiFoArr, &peekNode));
    EXPECT_EQ(5,peekNode.size);
    EXPECT_TRUE(  fifo_arr_pull(&FiFoArr, &rdNode));
    EXPECT_EQ(5,rdNode.size);
    EXPECT_EQ_MEM(arr5, rdNode.pArr, 5);

    EXPECT_EQ(0, fifo_arr_get_count( &FiFoArr));

    EXPECT_FALSE(fifo_arr_pull(&FiFoArr, &rdNode));
    return true;
}

bool test_fifo_array_types(void){

    EXPECT_EQ(8,sizeof(Array_t));
    return true;
}
