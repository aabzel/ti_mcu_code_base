#include "unit_test_run.h"

#include <inttypes.h>
#include <stddef.h>

#ifdef HAS_DEV_ERROR_HANDLER
#include "dev_error_handler.h"
#endif
#include "io_utils.h"
#ifdef HAS_CLI
#include "log.h"
#endif
#include "none_blocking_pause.h"
#include "print_buffer.h"
#include "terminal_codes.h"
#include "unit_test.h"
#include "unit_test_info.h"

#define MAX_FAILED_TEST 100U
static uint32_t failed_tests[MAX_FAILED_TEST];

static uint32_t failed_tests_count;
bool test_failed;

static void add_to_failed(uint32_t index);

static void add_to_failed(uint32_t index) {
    if(failed_tests_count < MAX_FAILED_TEST) {
        failed_tests[failed_tests_count] = index;
        failed_tests_count++;
    }
}

uint32_t unit_test_run(uint32_t index) {
    uint32_t count;
    const unit_test_info_t* ui = get_unit_test(index);
    if(NULL == ui) {
#ifdef HAS_CLI
        LOG_ERROR(SYS, "Invalid unit_test number %" PRIu32, index + 1U);
#endif
        count = 0U;
    } else {
#ifdef HAS_CLI
        io_printf("************* Run test %s.%" PRIu32 "/%" PRIu32 CRLF, ui->name, index + 1U, get_unit_test_count());
        print_buf_stream_en = true;
#endif
        test_failed = false;
#ifdef HAS_DEV_ERROR_HANDLER
        dev_errors_clear();
#endif
        test_failed = !ui->utest_func();
#ifdef HAS_CLI
        print_buf_stream_en = false;
#endif
#ifdef EMBEDDED_TEST
        log_zero_time = false;
#endif

#ifdef HAS_DEV_ERROR_HANDLER
        if(dev_errors_count_total != 0U) {
            LOG_ERROR(SYS, "Runtime error check detected. Mark test as failed. dev_errors_count_total=%" PRIu32,
                      dev_errors_count_total);
            dev_errors_dump();
            test_failed = true;
        }
#endif
        if(test_failed) {
#ifdef HAS_CLI
            io_puts("!ERRTEST" CRLF);
            io_flush();
#endif
            add_to_failed(index);
        } else {
#ifdef HAS_CLI
            io_puts("!OKTEST" CRLF);
            io_flush();
#endif
        }
        count = 1U;
#ifdef HAS_NORTOS
        wait_in_loop_ms(10);
#endif
    }
    return count;
}

void failed_tests_reset(void) { failed_tests_count = 0U; }

#ifdef HAS_CLI
void failed_tests_print(void) {
    if(0U != failed_tests_count) {
        uint32_t index;
        io_printf(VT_SETCOLOR_RED "%" PRIu32 "/%" PRIu32 " test failed!" CRLF, failed_tests_count,
                  get_unit_test_count());
        for(index = 0U; index < failed_tests_count; index++) {
            if(index >= MAX_FAILED_TEST) {
                break;
            }
            const unit_test_info_t* ui = get_unit_test(failed_tests[index]);
            io_printf("Failed: %s.%" PRIu32 CRLF, ui->name, failed_tests[index] + 1U);
        }
        io_puts(VT_SETCOLOR_NORMAL);
    } else {
        io_puts(VT_SETCOLOR_GREEN "All tests passed!" VT_SETCOLOR_NORMAL CRLF);
    }
}
#endif /*HAS_CLI*/
