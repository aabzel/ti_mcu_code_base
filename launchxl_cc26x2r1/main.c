
#include <stdint.h>
#include <stddef.h>

#include <NoRTOS.h>

#include <ti/drivers/Board.h>

#include "hw_init.h"
#include "sw_init.h"

int main(void){
    hw_init();
    sw_init();
    common_main_loop();
    while (1) {}
}
