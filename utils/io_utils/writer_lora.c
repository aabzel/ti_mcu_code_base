#include "writer_lora.h"

#include <stdbool.h>

#include "fifo_char.h"
#include "sys_config.h"
#include "lora_drv.h"

static char dbg_lora_o_data[LORA_TX_ARRAY_SIZE];

generic_writer_t dbg_lora_o = {
    .s = {writer_putc, writer_puts},
    .in_transmit = 0,
    .lost_char_count = 0,
    .total_char_count = 0,
    .error_count = 0,
    .fifo = {.fifoState={.size=sizeof(dbg_lora_o_data),
                         .start=0,
                         .end=0,
                         .count=0,
                         .errors=0},
             .array=dbg_lora_o_data,
             .initDone=true},
    .f_transmit = lora_writer_transmit,
};

bool lora_writer_transmit(struct generic_writer_s* writer) {
    bool res = false;
    fifo_index_t cur_sise = fifo_get_count(&writer->fifo);
    if( 120 < cur_sise ){
        char data[300] = ""; /*TODO Determine min size*/
        res = fifo_pull_array(&writer->fifo, data, (uint16_t*)&writer->in_transmit);
        if(0 < writer->in_transmit) {
            writer->total_char_count += writer->in_transmit;
            if(LORA_TX_ARRAY_SIZE < writer->in_transmit) {
                writer->in_transmit = LORA_TX_ARRAY_SIZE;
            }


            res = lora_send_queue((uint8_t*)data, writer->in_transmit);
            if(false==res){
                LoRaInterface.err_cnt++;
            }
        }
    }
    return res;
}
