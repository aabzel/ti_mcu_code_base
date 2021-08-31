

#ifndef LORA_HANDLER_H
#define LORA_HANDLER_H

#include <stdbool.h>
#include <stdint.h>

bool lora_proc_payload(uint8_t* rx_payload, uint8_t rx_size);

#endif /* LORA_HANDLER_H  */
