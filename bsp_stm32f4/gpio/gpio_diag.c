#include "gpio_diag.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bit_utils.h"
#include "gpio_drv.h"

const char* GpioPort2str(uint8_t port_num){
    const char* name = "_";
    switch(port_num){
        case PORT_A: name = "A"; break;
        case PORT_B: name = "B"; break;
        case PORT_C: name = "C"; break;
        case PORT_D: name = "D"; break;
        case PORT_E: name = "E"; break;
        case PORT_F: name = "F"; break;
        case PORT_G: name = "G"; break; 
        case PORT_H: name = "H"; break;
    }
    return name;
}



const char* get_gpio_mode(uint8_t pin) {
    char* name = "_";
    return name;
}
#if 0
static const char* port_ip2str(uint8_t code) {
    char* name = "no";
    return name;
}
static const char* iomode2str(uint8_t code) {
    const char* name = "no";

    return name;
}
static const char* edge2str(uint8_t edge_code) {
    const char* edge_name = "no";
 
    return edge_name;
}
#endif

const char* get_gpio_alter_fun(uint8_t dio_pin) {
    const char* name = "no";

    return name;
}

const char* get_gpio_pull_mode(uint8_t dio_pin) {
    char* pull_name = "no";

    return pull_name;
}


const char* get_gpio_type(uint8_t dio_pin) {
    const char* iomode_name = "no";

    return iomode_name;
}


const char* get_gpio_edge(uint8_t dio_pin) {
    const char* edge_name = "no";

    return edge_name;
}

const char* get_pin_dir(uint8_t dioNumber) {
    static char dir[4] = "";

    return dir;
}
