#ifndef TBFP_DIAG_H
#define TBFP_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "tbfp_protocol.h"

char *tbfp_id2str(uint8_t id);
bool tbfp_print_ping_frame(TbfPingFrame_t *pingFrame);

#ifdef __cplusplus
}
#endif

#endif /* TBFP_DIAG_H */
