#ifndef UBX_DIAG_H
#define UBX_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

char* class2str(uint8_t class_id);
bool ubx_print_frame(uint8_t* frame);

#ifdef __cplusplus
}
#endif

#endif /* UBX_DIAG_H */
