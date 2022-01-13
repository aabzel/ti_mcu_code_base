#include "timer_utils.h"

#ifdef HAS_CLOCK
#include "clocks.h"
#endif

void delay_us(uint32_t delay_in_us) {
#ifdef HAS_CLOCK
  uint64_t stop = get_time_us() + delay_in_us;
  while (get_time_us() < stop) {
  }
#endif
}

void delay_wd(uint32_t delay_in_ms) {
#ifdef HAS_CLOCK
  uint64_t stop = get_time_ms64() + delay_in_ms;
  while (get_time_ms64() < stop) {
   // wdt_reset_all();
  }
#endif
}

uint64_t get_time_tick64(void) {
    uint64_t val=0;
#ifdef HAS_CLOCK
    val=getRunTimeCounterValue64();
#endif
    return val;
}

uint64_t tick2us(uint64_t tick) {
    return tick / COUNTER_FREQ;
}
uint64_t runtime_2_us(uint64_t rtc) { return rtc / COUNTER_FREQ; }

uint64_t us2tick(uint64_t ms64) { return ms64 * COUNTER_FREQ; }

uint64_t get_runtime_counter(void) {
    uint64_t val=0;
#ifdef HAS_CLOCK
    val = getRunTimeCounterValue64();
#endif
    return val;
}

