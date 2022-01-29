#include "writer_config.h"

#ifdef HAS_UART
generic_writer_t *curWriterPtr=&dbg_o;
#endif
#ifdef X86_64
generic_writer_t *curWriterPtr=&std_out_o;
#endif

bool writer_init(void){
    bool res  = false;
#ifdef HAS_UART
    curWriterPtr = &dbg_o;
    res  = true;
#endif
#ifdef X86_64
    curWriterPtr = &std_out_o;
    res  = true;
#endif
    return res;
}
