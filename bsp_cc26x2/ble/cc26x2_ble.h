/*
   @brief This file contains the BLE sample application
        definitions and prototypes.
   Target Device: cc13xx_cc26xx
*/

#ifndef BLE_APP_H
#define BLE_APP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <ti/sysbios/knl/Queue.h>

#include <bcomdef.h>

extern void bluetooth_create_task(void);//ProjectZero_createTask

#ifdef __cplusplus
}
#endif

#endif /* BLE_APP_H */
