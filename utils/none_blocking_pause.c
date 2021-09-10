#include "none_blocking_pause.h"

#include "clocks.h"
#include "common_functions.h"
#include "task_info.h"
#include "timer_utils.h"

/*none blocking wait for self test and polling*/
bool wait_in_loop_ms(uint64_t wait_pause_ms) {
  uint64_t start_ms = 0U;
  uint64_t curr_ms = 0U;
  bool res = false;
  start_ms = get_time_ms64();
  bool loop = true;
  uint64_t loop_start_time_us = 0;
  while (loop) {
    iteration_cnt++;                                                                                                   \
    loop_start_time_us = get_time_us();                                                                       \
    common_loop(loop_start_time_us);

    curr_ms = get_time_ms64();
    if (wait_pause_ms < (curr_ms - start_ms)) {
      res = true;
      loop = false;
      break;
    }
  }
  return res;
}

bool wait_ms(uint64_t wait_pause_ms) {
  uint64_t start_ms = 0U;
  uint64_t curr_ms = 0U;
  bool res = false;
  start_ms = get_time_ms64();
  bool loop = true;
  uint64_t diff_ms = true;
  while (loop) {
    curr_ms = get_time_ms64();
    diff_ms = curr_ms - start_ms;
    if (wait_pause_ms < diff_ms) {
      res = true;
      loop = false;
      break;
    }
  }
  return res;
}

/*none blocking wait for self test and poling*/
bool wait_us(uint64_t wait_pause_us) {
  uint64_t start_us = 0U;
  uint64_t curr_us = 0U;
  uint64_t diff_us = 0U;
  bool res = false;
  start_us = get_time_us();
  bool loop = true;
  while (loop) {
    curr_us = get_time_us();
    diff_us = curr_us - start_us;
    if (wait_pause_us < diff_us) {
      res = true;
      loop = false;
      break;
    }
  }
  return res;
}
