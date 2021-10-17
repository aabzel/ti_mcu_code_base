#ifndef IMPLEMENT_QUEUE_H
#define IMPLEMENT_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fifo_char.h"

typedef struct {
   Fifo_array_t FiFoChar; 
} MyQueue;

MyQueue* myQueueCreate() ;
void myQueuePush(MyQueue* obj, int x) ;
int myQueuePop(MyQueue* obj) ;
int myQueuePeek(MyQueue* obj) ;
bool myQueueEmpty(MyQueue* obj) ;
void myQueueFree(MyQueue* obj);

#ifdef __cplusplus
}
#endif

#endif /* IMPLEMENT_QUEUE_H  */