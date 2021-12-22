
#ifndef _CLI_SERVICE_H_
#define _CLI_SERVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include <bcomdef.h>


#define CLI_SERVICE_SERV_UUID 0x1130
#define CLI_SERVICE_SERV_UUID_BASE128(uuid) 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0xB0, 0x00, 0x40, 0x51, 0x04, LO_UINT16(uuid), HI_UINT16(uuid), 0x00, \
    0xF0

// String Characteristic defines
#define CS_STRING_ID                 0
#define CS_STRING_UUID               0x1131
#define CS_STRING_UUID_BASE128(uuid) 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0xB0, 0x00, 0x40, 0x51, 0x04, LO_UINT16(uuid), HI_UINT16(uuid), 0x00, 0xF0
#define CS_STRING_LEN                40
#define CS_STRING_LEN_MIN            0

// Stream Characteristic defines
#define CS_STREAM_ID                 1
#define CS_STREAM_UUID               0x1132
#define CS_STREAM_UUID_BASE128(uuid) 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0xB0, 0x00, 0x40, 0x51, 0x04, LO_UINT16(uuid), HI_UINT16(uuid), 0x00, 0xF0
#define CS_STREAM_LEN                20
#define CS_STREAM_LEN_MIN            0


// Callback when a characteristic value has changed
typedef void (*CliServiceChange_t)(uint16_t connHandle, uint8_t paramID,
                                    uint16_t len, uint8_t *pValue);

typedef struct
{
    CliServiceChange_t pfnChangeCb;          // Called when characteristic value changes
    CliServiceChange_t pfnCfgChangeCb;       // Called when characteristic CCCD changes
} CliServiceCBs_t;


/*
 * CliService_AddService- Initializes the CliService service by registering
 *          GATT attributes with the GATT server.
 *
 *    rspTaskId - The ICall Task Id that should receive responses for Indications.
 */
extern bStatus_t CliService_AddService(uint8_t rspTaskId);

/*
 * CliService_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t CliService_RegisterAppCBs(CliServiceCBs_t *appCallbacks);

/*
 * CliService_SetParameter - Set a CliService parameter.
 *
 *    param - Profile parameter ID
 *    len   - length of data to write
 *    value - pointer to data to write.  This is dependent on
 *            the parameter ID and may be cast to the appropriate
 *            data type (example: data type of uint16_t will be cast to
 *            uint16_t pointer).
 */
extern bStatus_t CliService_SetParameter(uint8_t param,
                                          uint16_t len,
                                          void *value);

/*
 * CliService_GetParameter - Get a CliService parameter.
 *
 *    param - Profile parameter ID
 *    len   - pointer to a variable that contains the maximum length that can be written to *value.
              After the call, this value will contain the actual returned length.
 *    value - pointer to data to write.  This is dependent on
 *            the parameter ID and may be cast to the appropriate
 *            data type (example: data type of uint16_t will be cast to
 *            uint16_t pointer).
 */
extern bStatus_t CliService_GetParameter(uint8_t param,
                                          uint16_t *len,
                                          void *value);


#ifdef __cplusplus
}
#endif

#endif /* _CLI_SERVICE_H_ */
