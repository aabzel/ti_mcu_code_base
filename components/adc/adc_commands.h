#ifndef ADC_COMMANDS_H
#define ADC_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool adc_diag_command(int32_t argc, char* argv[]);

#define ADC_COMMANDS                                                                  \
        SHELL_CMD("adc_diag", "ain", adc_diag_command, "ADC diag") ,                   \

#ifdef __cplusplus
}
#endif

#endif /* ADC_COMMANDS_H */
