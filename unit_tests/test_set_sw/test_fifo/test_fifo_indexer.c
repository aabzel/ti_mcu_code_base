#include "test_fifo_indexer.h"

#include "fifo_index.h"
#include "unit_test_check.h"

bool test_fifo_index_init(void) {
    fifo_index_info_t ri;
    memset(&ri, 0xFF, sizeof((void*)&ri));
    fifo_index_init(&ri, RI_SIZE);
    EXPECT_EQ(0, ri.end);
    EXPECT_EQ(0, ri.start);
    EXPECT_EQ(0, fifo_index_get_used(&ri));
    EXPECT_EQ(RI_SIZE, ri.size);
    EXPECT_TRUE(fifo_index_valid(&ri));
    return true;
}

bool test_fifo_index_add(void) {
    fifo_index_info_t ri;
    fifo_index_t index;
    memset(&ri, 0xFF, sizeof(ri));
    fifo_index_init(&ri, RI_SIZE);
    for(int i = 0; i < RI_SIZE; i++) {
        index = fifo_index_add(&ri);
        EXPECT_EQ(i, index);
        EXPECT_EQ(i + 1, fifo_index_get_used(&ri));
        EXPECT_TRUE(fifo_index_valid(&ri));
    }
    index = fifo_index_add(&ri);
    EXPECT_EQ(RING_INVALID_INDEX, index);
    EXPECT_TRUE(fifo_index_valid(&ri));
    index = fifo_index_get(&ri);
    EXPECT_EQ(0, index);
    EXPECT_TRUE(fifo_index_valid(&ri));
    index = fifo_index_add(&ri);
    EXPECT_EQ(0, index);
    EXPECT_TRUE(fifo_index_valid(&ri));

    for(int i = 1; i < 1000; i++) {
        index = fifo_index_add(&ri);
        EXPECT_EQ(RING_INVALID_INDEX, index);
        EXPECT_TRUE(fifo_index_valid(&ri));
        index = fifo_index_get(&ri);
        EXPECT_EQ(i % RI_SIZE, index);
        EXPECT_TRUE(fifo_index_valid(&ri));
        index = fifo_index_add(&ri);
        EXPECT_EQ(i % RI_SIZE, index);
        EXPECT_TRUE(fifo_index_valid(&ri));
    }
    EXPECT_TRUE(fifo_index_valid(&ri));
    return true;
}

bool test_fifo_index_get(void) {
    fifo_index_info_t ri;
    fifo_index_t index;
    memset(&ri, 0xFF, sizeof(ri));
    fifo_index_init(&ri, RI_SIZE);
    index = fifo_index_get(&ri);
    EXPECT_EQ(RING_INVALID_INDEX, index);
    EXPECT_TRUE(fifo_index_valid(&ri));
    for(int i = 0; i < RI_SIZE; i++) {
        index = fifo_index_add(&ri);
        EXPECT_EQ(i, index);
        EXPECT_EQ(i + 1, fifo_index_get_used(&ri));
        EXPECT_TRUE(fifo_index_valid(&ri));
    }
    for(int i = 0; i < RI_SIZE; i++) {
        index = fifo_index_get(&ri);
        EXPECT_EQ(i, index);
        EXPECT_EQ(RI_SIZE - (i + 1), fifo_index_get_used(&ri));
        EXPECT_TRUE(fifo_index_valid(&ri));
    }
    index = fifo_index_get(&ri);
    EXPECT_EQ(RING_INVALID_INDEX, index);
    EXPECT_TRUE(fifo_index_valid(&ri));
    return true;
}

bool test_fifo_index_continuus_used(void) {
    fifo_index_info_t ri;
    memset(&ri, 0xFF, sizeof(ri));
    fifo_index_init(&ri, RI_SIZE);
    EXPECT_EQ(0, fifo_index_continuus_used_size(&ri));
    EXPECT_TRUE(fifo_index_valid(&ri));
    for(int i = 0; i < RI_SIZE; i++) {
        fifo_index_add(&ri);
        EXPECT_EQ(i + 1, fifo_index_continuus_used_size(&ri));
        EXPECT_TRUE(fifo_index_valid(&ri));
    }
    for(int i = 0; i < RI_SIZE; i++) {
        fifo_index_get(&ri);
        EXPECT_EQ(RI_SIZE - (i + 1), fifo_index_continuus_used_size(&ri));
        EXPECT_TRUE(fifo_index_valid(&ri));
    }
    EXPECT_EQ(0, fifo_index_continuus_used_size(&ri));
    fifo_index_add(&ri);
    for(int i = 0; i < RI_SIZE * 5; i++) {
        fifo_index_add(&ri);
        fifo_index_get(&ri);
        EXPECT_EQ(1, fifo_index_continuus_used_size(&ri));
        EXPECT_TRUE(fifo_index_valid(&ri));
    }
    return true;
}

bool test_fifo_index_free(void) {
    fifo_index_info_t ri;
    fifo_index_t index;
    memset(&ri, 0xFF, sizeof(ri));
    fifo_index_init(&ri, RI_SIZE);
    for(int i = 0; i < RI_SIZE * 5; i++) {
        index = fifo_index_add(&ri);
        EXPECT_EQ(i % RI_SIZE, index);
        fifo_index_free(&ri, 1);
        EXPECT_EQ(0, fifo_index_get_used(&ri));
        EXPECT_TRUE(fifo_index_valid(&ri));
    }
    fifo_index_init(&ri, RI_SIZE);
    EXPECT_TRUE(fifo_index_valid(&ri));

    fifo_index_init(&ri, RI_SIZE);
    for(int i = 0; i < RI_SIZE / 2; i++) {
        index = fifo_index_add(&ri);
        EXPECT_EQ(i % RI_SIZE, index);
    }
    fifo_index_free(&ri, RI_SIZE / 2);
    EXPECT_TRUE(fifo_index_valid(&ri));
    EXPECT_EQ(0, fifo_index_get_used(&ri));

    for(int i = 0; i < RI_SIZE; i++) {
        index = fifo_index_add(&ri);
    }
    fifo_index_free(&ri, RI_SIZE);
    EXPECT_TRUE(fifo_index_valid(&ri));
    EXPECT_EQ(0, fifo_index_get_used(&ri));
    return true;
}
