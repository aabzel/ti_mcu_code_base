#ifndef ADC_COMMANDS_H
#define ADC_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool adc_all_command(int32_t argc, char* argv[]);
bool adc_start_command(int32_t argc, char* argv[]);
bool adc_wait_fifo_command(int32_t argc, char* argv[]);
bool adc_diag_command(int32_t argc, char* argv[]);
bool adc_sel_in_command(int32_t argc, char* argv[]);
bool adc_trig_command(int32_t argc, char* argv[]);

#define ADC_COMMANDS                                                                                                   \
    SHELL_CMD("adc_sel", "asl", adc_sel_in_command, "ADC input select"),                                               \
        SHELL_CMD("adc_start", "ast", adc_start_command, "ADC start"),                                                 \
        SHELL_CMD("adc_all", "aas", adc_all_command, "ADC all start"),                                                 \
        SHELL_CMD("adc_trig", "atg", adc_trig_command, "ADC trig"),                                                    \
        SHELL_CMD("adc_wait", "awf", adc_wait_fifo_command, "ADC wait fifo"),                                          \
        SHELL_CMD("adc_diag", "ain", adc_diag_command, "ADC diag"),

#ifdef __cplusplus
}
#endif

#endif /* ADC_COMMANDS_H */
