#include "gpio_diag.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bit_utils.h"
#include "gpio_drv.h"

const char* get_gpio_mode(uint8_t pin) {
    char* name = "_";

    return name;
}

static const char* port_ip2str(uint8_t code) {
    char* name = "no";
  
    return name;
}

const char* get_gpio_alter_fun(uint8_t dio_pin) {
    const char* name = "no";

    return name;
}

const char* get_gpio_pull_mode(uint8_t dio_pin) {
    char* pull_name = "no";

    return pull_name;
}

static const char* iomode2str(uint8_t code) {
    const char* name = "no";

    return name;
}

const char* get_gpio_type(uint8_t dio_pin) {
    const char* iomode_name = "no";

    return iomode_name;
}

static const char* edge2str(uint8_t edge_code) {
    const char* edge_name = "no";
  
    return edge_name;
}

const char* get_gpio_edge(uint8_t dio_pin) {
    const char* edge_name = "no";

    return edge_name;
}

const char* gpio_dir2str(DioDir_t gpio_dir) {
    const char* name = "undef";

    return name;
}
