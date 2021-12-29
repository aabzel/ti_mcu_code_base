#ifndef SX1262_TYPES_H
#define SX1262_TYPES_H

#include <stdint.h>


typedef union uSx1262Status_t{
      uint8_t byte;
      struct {
          uint8_t reserved1 :1;      /*bit 0  :*/
          uint8_t command_status:3;  /*bit 3-1:*/
          uint8_t chip_mode:3;       /*bit 6-4:*/
          uint8_t reserved7:1;       /*bit 7:  */
      };
} Sx1262Status_t;

/*Status Bytes Definition*/
typedef enum eCommandStatus_t{
   COM_STAT_DATA_AVAIL= 0x2,   /*Data is available to host*/
   COM_STAT_COM_TIMEOUT= 0x3,  /*Command timeout2 */
   COM_STAT_COM_PROC_ERR= 0x4, /*Command processing error3*/
   COM_STAT_EXE_ERR =0x5,      /*Failure to execute command4*/
   COM_STAT_COM_TX_DONE= 0x6,  /*Command TX done5 */
}CommandStatus_t;



#endif /* SX1262_TYPES_H  */
