#include "timer_utils.h"

#include "clocks.h"

void delay_us(uint32_t delay_in_us) {
  uint64_t stop = get_time_us() + delay_in_us;
  while (get_time_us() < stop) {
  }
}

void delay(uint32_t delay_in_ms) {
  uint64_t stop = get_time_ms64() + delay_in_ms;
  while (get_time_ms64() < stop) {
  }
}

void delay_wd(uint32_t delay_in_ms) {
  uint64_t stop = get_time_ms64() + delay_in_ms;
  while (get_time_ms64() < stop) {
   // wdt_reset_all();
  }
}

uint64_t get_time_tick64(void) { return getRunTimeCounterValue64(); }

uint64_t tick2us(uint64_t tick) { return tick / COUNTER_FREQ; }

uint64_t us2tick(uint64_t ms64) { return ms64 * COUNTER_FREQ; }

uint64_t get_runtime_counter(void) { return getRunTimeCounterValue64(); }

uint64_t runtime_2_us(uint64_t rtc) { return rtc / COUNTER_FREQ; }
