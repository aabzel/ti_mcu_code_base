
#include "freertos_init.h"
#include "FreeRTOS.h"

#include <stdbool.h>
#include <stdint.h>

//#include "cmsis_os.h"
#include "io_utils.h"
#include "log.h"
#include "main.h"
#include "semphr.h"
#include "task.h"

#ifdef HAS_EXT_RAM_EMUL
#include "ext_ram_sim_drv.h"
#endif /*HAS_EXT_RAM_EMUL*/

#ifdef HAS_CLI
#include "cli_manager.h"
#endif /*HAS_CLI*/

TaskHandle_t CliTaskHandle;
TaskHandle_t Task1Handle;
TaskHandle_t Task2Handle;

uint32_t wr_period_ms = 2;
uint32_t rd_period_ms = 2;


void StartTaskReader(void* argument) {
    bool res = false;
    uint32_t read_cur = 0;
    uint32_t read_prev = 0;
    uint32_t err_cnt = 0;
    uint32_t diff = 0;
    BaseType_t ret = 0;
    uint32_t sem_err_cnt = 0;
    for(;;) {
        ret = xSemaphoreTake(ext_ram_sem, sem_wait);
        if(pdTRUE == ret) {
            res = ext_ram_read(0, (uint8_t*)&read_cur, sizeof(read_cur));
            if(res) {
                diff = read_cur - read_prev;
                if(0 != diff) {
                    io_printf("read cnt: %u" CRLF, read_cur);
                }
                //if(1 < diff) {
                //    LOG_ERROR(SYS, "cur %u prev %u diff %u", read_prev, read_cur, diff);
                //}
                read_prev = read_cur;
            } else {
                LOG_ERROR(SYS, "Read error %u", err_cnt);
                err_cnt++;
            }
            xSemaphoreGive(ext_ram_sem);
        }else{
           sem_err_cnt++;
        }
        vTaskDelay(rd_period_ms);
    }
}

#if 0
void StartTaskWriter(void* argument) {
    bool res = false;
    uint32_t cnt = 0;
    uint32_t err_cnt = 0;
    uint32_t sem_err_cnt = 0;
    BaseType_t ret = 0;
    for(;;) {
        cnt++;
        ret = xSemaphoreTake(ext_ram_sem, sem_wait);
        if(pdTRUE == ret) {
            res = ext_ram_write(0, (uint8_t*)&cnt, sizeof(cnt));
            if(false == res) {
                LOG_ERROR(SYS, "Write error %u", err_cnt);
                err_cnt++;
            }
            xSemaphoreGive(ext_ram_sem);
        }else{
           sem_err_cnt++;
        }

        vTaskDelay(wr_period_ms);
    }
}
#endif

void CliTask(void* argument) {

    for(;;) {
#ifdef HAS_CLI
        cli_process();
#endif /*HAS_CLI*/
        vTaskDelay(10);
    }
}

bool freertos_init(void) {
    bool out_res = true;
    BaseType_t stat = pdPASS;
#if 0
    stat = xTaskCreate((TaskFunction_t)StartTaskWriter, "Writer", (uint16_t)128 * 4, NULL, configMAX_PRIORITIES/2, &Task1Handle);
    if(pdPASS != stat) {
        out_res = false;
    }
#endif

    stat = xTaskCreate((TaskFunction_t)StartTaskReader, "Reader", (uint16_t)128 * 4, NULL, configMAX_PRIORITIES/2, &Task2Handle);
    if(pdPASS != stat) {
        out_res = false;
    }

    stat = xTaskCreate((TaskFunction_t)CliTask, "CLI", (uint16_t)128 * 8, NULL, configMAX_PRIORITIES-2, &CliTaskHandle);
    if(pdPASS != stat) {
        out_res = false;
    }

    return out_res;
}
