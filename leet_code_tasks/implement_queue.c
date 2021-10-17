
#include "implement_queue.h"

char inArray[200]="";
MyQueue FiFoCharInst;

MyQueue* myQueueCreate() {
   MyQueue* FiFo=NULL;
   bool res= fifo_init(&FiFoCharInst.FiFoChar, inArray, sizeof(inArray));
   if(res){
	   FiFo = &FiFoCharInst;
   }   
   return FiFo;
}

void myQueuePush(MyQueue* obj, int x) {
   fifo_push(&obj->FiFoChar, (char) x);
}

int myQueuePop(MyQueue* obj) {
  int x=0;
  char	outChar = 0;
  bool res= fifo_pull(&obj->FiFoChar, &outChar);
  if(res){
	  x = outChar;
  }
  return x;
}

int myQueuePeek(MyQueue* obj) {
  int x = 0;
  char	outChar = 0;
  bool res= fifo_peek(&obj->FiFoChar, &outChar);
  if(res){
	  x = outChar;
  }
  return x;
}

bool myQueueEmpty(MyQueue* obj) {
    bool res = false;
    fifo_index_t size = fifo_get_count(&obj->FiFoChar);
    if(0==size){
       res = true;
    }
    return res;
}

void myQueueFree(MyQueue* obj) {
    
}
