#include "writer_config.h"

generic_writer_t *curWriterPtr=&dbg_o;;

bool writer_init(void){
    bool res  = false;
    curWriterPtr = &dbg_o;
    return res;
}
