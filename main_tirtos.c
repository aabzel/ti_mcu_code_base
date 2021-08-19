
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>

#include <ti/drivers/Board.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

#include "clocks.h"
#include "common_functions.h"
#include "debug_info.h"
#ifdef HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif /*HAS_HEALTH_MONITOR*/
#include "gpio_drv.h"
#include "hw_init.h"
#include "io_utils.h"
#include "log.h"
#include "sw_init.h"
#include "uart_drv.h"

static bool sys_init(void) {
    bool res = true;
    res = hw_init() && res;
    res = sw_init() && res;
    return res;
}

//extern void *temperatureThread(void *arg0);
extern void *cliThread(void *arg0);

/* Stack size in bytes. Large enough in case debug kernel is used. */
#define THREADSTACKSIZE    1024

int main(void) {
    bool res = true;
    pause_ms(2000);

    res = sys_init() && res;

    io_printf(CRLF "init [%s]" CRLF, (true == res) ? "OK!" : "Error!");
#ifdef HAS_HEALTH_MONITOR
    HealthMon.init_error = !res;
#endif /*HAS_HEALTH_MONITOR*/

    print_version();
    print_sys_info();

    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

 

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, cliThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }
#if 0
    /*
     *  Let's make the temperature thread a higher priority .
     *  Higher number means higher priority in TI-RTOS.
     */
    priParam.sched_priority = 2;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set priority */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, temperatureThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }
#endif

    /* Start the TI-RTOS scheduler */
    BIOS_start();
    /*Unreachable line*/
    while(1) {
    }
}
