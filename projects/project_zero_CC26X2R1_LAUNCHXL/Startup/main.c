/*
   @file  main.c
   @brief main entry of the BLE stack sample application.
   Target Device: cc13xx_cc26xx
 */

#include <stdint.h>

#include <xdc/runtime/Error.h>

#include <ti/sysbios/knl/Clock.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/UART.h>

// Comment this in to use xdc.runtime.Log, but also remove UartLog_init below.
//#include <xdc/runtime/Log.h>
#include <ti/common/cc26xx/uartlog/UartLog.h> // Comment out to use xdc Log.

#include <common/cc26xx/flash_interface/flash_interface.h>
#include "find_stack_entry.h"

#include <icall.h>

#include "hal_assert.h"
#include "bcomdef.h"
#include "project_zero.h"

#ifndef USE_DEFAULT_USER_CFG
#include "ble_user_config.h"
icall_userCfg_t user0Cfg = BLE_USER_CFG;
#endif // USE_DEFAULT_USER_CFG

// The stack's image header is found at runtime, available for entire app to use
const imgHdr_t *stackImageHeader = NULL;

extern void AssertHandler(uint8_t assertCause,
                          uint8_t assertSubcause);

int main(){
  /* Register Application callback to trap asserts raised in the Stack */
  RegisterAssertCback(AssertHandler);

  Board_initGeneral();

#if !defined( POWER_SAVING )
  /* Set constraints for Standby, powerdown and idle mode */
  // PowerCC26XX_SB_DISALLOW may be redundant
  Power_setConstraint(PowerCC26XX_SB_DISALLOW);
  Power_setConstraint(PowerCC26XX_IDLE_PD_DISALLOW);
#endif // POWER_SAVING

    /* Update User Configuration of the stack */
    user0Cfg.appServiceInfo->timerTickPeriod = Clock_tickPeriod;
    user0Cfg.appServiceInfo->timerMaxMillisecond = ICall_getMaxMSecs();

    /* Initialize the RTOS Log formatting and output to UART in Idle thread.
     * Note: Define xdc_runtime_Log_DISABLE_ALL and remove define UARTLOG_ENABLE
     *       to remove all impact of Log statements.
     * Note: NULL as Params gives 115200,8,N,1 and Blocking mode */
    UART_init();
    UartLog_init(UART_open(CONFIG_DISPLAY_UART, NULL));

    ICall_init();

#ifndef STACK_LIBRARY
    {
        /* Find stack entry page */
        uint32_t stackAddr = findStackBoundaryAddr();

        if(stackAddr == 0xFFFFFFFF)
        {
            // If we cannot find the stack start address, exit
            ICall_abort();
        }

        /* set the stack image header based on the stack addr */
        stackImageHeader = (imgHdr_t *)stackAddr;

        /* Start tasks of external images - Priority 5 */
        const ICall_RemoteTask_t remoteTaskTbl[] =
        {
            (ICall_RemoteTaskEntry) (stackImageHeader->prgEntry),
            5,
            1000,
            &user0Cfg
        };

        /* Start tasks of external images - Priority 5 */
        ICall_createRemoteTasksAtRuntime((ICall_RemoteTask_t *) remoteTaskTbl,
                                         (sizeof(remoteTaskTbl) /
                                          sizeof(ICall_RemoteTask_t)));
    }
#else

    /* Start tasks of external images - Priority 5 */
    ICall_createRemoteTasks();
#endif

    ProjectZero_createTask();

    /* enable interrupts and start SYS/BIOS */
    BIOS_start();

    return(0);
}

/*******************************************************************************
 * @fn          AssertHandler
 *
 * @brief       This is the Application's callback handler for asserts raised
 *              in the stack.  When EXT_HAL_ASSERT is defined in the Stack Wrapper
 *              project this function will be called when an assert is raised,
 *              and can be used to observe or trap a violation from expected
 *              behavior.
 *
 *              As an example, for Heap allocation failures the Stack will raise
 *              HAL_ASSERT_CAUSE_OUT_OF_MEMORY as the assertCause and
 *              HAL_ASSERT_SUBCAUSE_NONE as the assertSubcause.  An application
 *              developer could trap any malloc failure on the stack by calling
 *              HAL_ASSERT_SPINLOCK under the matching case.
 *
 *              An application developer is encouraged to extend this function
 *              for use by their own application.  To do this, add hal_assert.c
 *              to your project workspace, the path to hal_assert.h (this can
 *              be found on the stack side). Asserts are raised by including
 *              hal_assert.h and using macro HAL_ASSERT(cause) to raise an
 *              assert with argument assertCause.  the assertSubcause may be
 *              optionally set by macro HAL_ASSERT_SET_SUBCAUSE(subCause) prior
 *              to asserting the cause it describes. More information is
 *              available in hal_assert.h.
 *
 * input parameters
 *
 * @param       assertCause    - Assert cause as defined in hal_assert.h.
 * @param       assertSubcause - Optional assert subcause (see hal_assert.h).
 *
 */
void AssertHandler(uint8_t assertCause, uint8_t assertSubcause)
{
    Log_error2(">>>STACK ASSERT Cause 0x%02x subCause 0x%02x",
               assertCause, assertSubcause);

    // check the assert cause
    switch(assertCause)
    {
    case HAL_ASSERT_CAUSE_OUT_OF_MEMORY:
        Log_error0("***ERROR***");
        Log_error0(">> OUT OF MEMORY!");
        break;

    case HAL_ASSERT_CAUSE_INTERNAL_ERROR:
        // check the subcause
        if(assertSubcause == HAL_ASSERT_SUBCAUSE_FW_INERNAL_ERROR)
        {
            Log_error0("***ERROR***");
            Log_error0(">> INTERNAL FW ERROR!");
        }
        else
        {
            Log_error0("***ERROR***");
            Log_error0(">> INTERNAL ERROR!");
        }
        break;

    case HAL_ASSERT_CAUSE_ICALL_ABORT:
        Log_error0("***ERROR***");
        Log_error0(">> ICALL ABORT!");
        //HAL_ASSERT_SPINLOCK;
        break;

    case HAL_ASSERT_CAUSE_ICALL_TIMEOUT:
        Log_error0("***ERROR***");
        Log_error0(">> ICALL TIMEOUT!");
        //HAL_ASSERT_SPINLOCK;
        break;

    case HAL_ASSERT_CAUSE_WRONG_API_CALL:
        Log_error0("***ERROR***");
        Log_error0(">> WRONG API CALL!");
        //HAL_ASSERT_SPINLOCK;
        break;

    default:
        Log_error0("***ERROR***");
        Log_error0(">> DEFAULT SPINLOCK!");
        //HAL_ASSERT_SPINLOCK;
    }

    return;
}

