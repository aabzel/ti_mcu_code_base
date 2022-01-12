/*
   @file  project_zero.h
   @brief This file contains the Project Zero sample application
        definitions and prototypes.
   Target Device: cc13xx_cc26xx
*/

#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/drivers/PIN.h>

#include <bcomdef.h>

/*
 * Task creation function for the Project Zero.
 */
extern void ProjectZero_createTask(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_H */
