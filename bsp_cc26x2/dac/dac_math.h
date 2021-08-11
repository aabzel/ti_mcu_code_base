#ifndef DAC_MATH_H
#define DAC_MATH_H

#include <stdbool.h>
#include <stdint.h>

#include "float_utils.h"

#define PI (3.14159265f)


float calc_pwm_sample (uint64_t curTimeUs, float inFrequency, float inPhaseMs, float inVoltageAmplitude,
                         float inYOffset, float inDutyCycle);

float calc_fence_sample (uint64_t inTimeUs, float inFrequency, float inPhaseMs, float inVoltageAmplitude,
                           float inOffset);
float calc_saw_sample (uint64_t inTimeUs, float inFrequency, float inPhaseMs, float inVoltageAmplitude,
                         float inOffset);
float calc_sin_sample (uint64_t inTimeUs, float inFrequency, float inPhaseMs, float inVoltageAmplitude,
                         float inOffset); 
uint16_t voltage2sample (float inVoltage, float inOapGain, uint16_t maxSampleVal, float dacVrefV);

#endif /* DAC_MATH_H */
