#include "dac_math.h"

#include <math.h>

float calc_pwm_sample (uint64_t cur_time_us, float in_frequency, float in_phase_ms, float in_voltage_amplitude,
                         float in_y_offset, float in_duty_cycle) {

    float line_val = 0.0f;
    float saw_val = 0.0f;

    if (100.0f < in_duty_cycle) {
        in_duty_cycle = 100.0f;
    }
    if (in_duty_cycle < 0.0f) {
        in_duty_cycle = 0.0f;
    }
    float amplitude = 0.0f;
    float amplitude_scaled = 0.0f;
    float current_time_ms = ((float)cur_time_us) / 1000.0f;
    line_val = ((current_time_ms + in_phase_ms) / 1000.0f) * in_frequency; /*tune frequency*/
    saw_val = fmodf (line_val, 1.0f);                                      /*tune DutyCycle*/
    amplitude = 0.0f;
    float val = (((1.0f - ((100.0f - in_duty_cycle)) / 100.0f)) - saw_val); /*tune DutyCycle*/
    /*tune amplitude*/
    if (0.0f < val) {
        amplitude = 1.0f;
    }
    amplitude_scaled = (in_voltage_amplitude * amplitude) + in_y_offset;

    return amplitude_scaled;
}

float calc_sin_sample (uint64_t in_time_us, float in_frequency, float in_phase_ms, float in_voltage_amplitude,
                         float in_offset) {
    float lineVal = 0.0f;
    float argument = 0.0f;
    float amplitude = 0.0f;
    float amplitudeScaled = 0.0f;
    float currentTimeMs = ((float)in_time_us) / 1000.0f;
    lineVal = ((currentTimeMs + in_phase_ms) / 1000.0f) * in_frequency; /*tune frequency*/
    argument = 2.0f * PI * lineVal;
    amplitude = (float)sin ((double)argument); /*tune DutyCycle*/
    /*tune amplitude*/
    amplitudeScaled = (in_voltage_amplitude * amplitude) + in_offset;
    /*outDacSample = cacl_sample (amplitudeScaled, BIG_OAP_GAIN);*/

    return amplitudeScaled;
}

float calc_saw_sample (uint64_t in_time_us, float in_frequency, float in_phase_ms, float in_voltage_amplitude,
                         float in_offset) {
    float argument = 0.0f;
    float amplitude = 0.0f;
    float amplitudeScaled = 0.0f;
    float currentTimeMs = ((float)in_time_us) / 1000.0f;
    argument = ((currentTimeMs + in_phase_ms) / 1000.0f) * in_frequency; /*tune frequency*/

    amplitude = (float)in_voltage_amplitude * fmod ((double)argument, (double)1.0f);
    /*tune amplitude*/
    amplitudeScaled = amplitude + in_offset;

    return amplitudeScaled;
}

float calc_fence_sample (uint64_t in_time_us, float in_frequency, float in_phase_ms, float in_voltage_amplitude,
                           float in_offset) {
    float argument = 0.0f;
    float amplitude = 0.0f;
    float amplitudeScaled = 0.0f;
    float currentTimeMs = ((float)in_time_us) / 1000.0f;
    argument = ((currentTimeMs + in_phase_ms) / 1000.0f) * in_frequency; /*tune frequency*/

    amplitude = 1.0f - fabs (fmod ((double)argument * 2.0f, 2.0f) - 1.0f);
    /*tune amplitude*/
    amplitudeScaled = in_voltage_amplitude * amplitude + in_offset;

    return amplitudeScaled;
}

uint16_t voltage2sample (float in_voltage, float inOapGain, uint16_t maxSampleVal, float dacVrefV) {
    uint16_t outSample = 0U;
    float maxPosVolgateForGain = inOapGain * dacVrefV;
    float codePerVoltsForGain = ((float) (maxSampleVal + 1.0f)) / maxPosVolgateForGain;
    if (maxSampleVal < (in_voltage * codePerVoltsForGain)) {
        outSample = maxSampleVal - 1U;
    } else {
        if ((in_voltage * codePerVoltsForGain) < 0.0f) {
            outSample = 0x0000U;
        } else {
            /*TODO: fix MISRA 2012 Rule 10.8: invalid cast from "float" to "uint16_t"      */
            outSample = (uint16_t) (in_voltage * codePerVoltsForGain);
        }
    }
    return outSample;
}
