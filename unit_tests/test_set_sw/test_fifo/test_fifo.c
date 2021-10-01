#include "test_fifo.h"

#include "fifo_char_buffer.h"
#include "unit_test_check.h"

bool test_fifo(void) {
    fifo_char_t testFifo;
    char fifoData[5];
    EXPECT_TRUE(fifo_char_init(&testFifo, fifoData, sizeof(fifoData)));
    EXPECT_TRUE(fifo_char_add(&testFifo, '0'));
    EXPECT_TRUE(fifo_char_add(&testFifo, '1'));
    EXPECT_TRUE(fifo_char_add(&testFifo, '2'));
    fifo_index_t size=0;
    char* prt = fifo_char_get_contiguous_block(&testFifo, &size) ;
    EXPECT_NE(NULL, prt) ;
    EXPECT_EQ(3, size) ;
    EXPECT_EQ('0', prt[0]) ;
    EXPECT_EQ('1', prt[1]) ;
    EXPECT_EQ('2', prt[2]) ;

    size=0;
    prt = fifo_char_get_contiguous_block(&testFifo, &size) ;
    EXPECT_NE(NULL, prt) ;
    EXPECT_EQ(0, size) ;

    return true;
}


bool test_fifo_overrun(void) {
    fifo_char_t testFifo;
    char fifoData[4];
    EXPECT_TRUE(fifo_char_init(&testFifo, fifoData, sizeof(fifoData)));
    EXPECT_TRUE(fifo_char_add(&testFifo, '1'));
    EXPECT_TRUE(fifo_char_add(&testFifo, '2'));
    EXPECT_TRUE(fifo_char_add(&testFifo, '3'));
    EXPECT_TRUE(fifo_char_add(&testFifo, '4'));
    EXPECT_FALSE(fifo_char_add(&testFifo, '5'));
    EXPECT_FALSE(fifo_char_add(&testFifo, '6'));

    fifo_index_t size = 0;
    char* prt = fifo_char_get_contiguous_block(&testFifo, &size) ;
    EXPECT_EQ(4, size);
    EXPECT_EQ('0', prt[0]);
    EXPECT_EQ('1', prt[1]);
    EXPECT_EQ('2', prt[2]);
    EXPECT_EQ('3', prt[3]);

    size = 0;
    prt = fifo_char_get_contiguous_block(&testFifo, &size) ;
    EXPECT_NE(NULL, prt) ;
    EXPECT_EQ(0, size) ;

    return true;
}

