/******************************************************************************

   @file  data_service.c

   @brief   This file contains the implementation of the service.

   Group: WCS, BTS
   Target Device: cc13x2_26x2

 ******************************************************************************
   
 Copyright (c) 2015-2021, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
   
   
 *****************************************************************************/


#include <string.h>


#include <ti/common/cc26xx/uartlog/UartLog.h>  // Comment out if using xdc Log

#include <icall.h>

/* This Header file contains all BLE API and icall structure definition */
#include "icall_ble_api.h"

#include "cli_service.h"


// cli_service Service UUID
CONST uint8_t CliServiceUUID[ATT_UUID_SIZE] =
{
    CLI_SERVICE_SERV_UUID_BASE128(CLI_SERVICE_SERV_UUID)
};

// String UUID
CONST uint8_t cs_StringUUID[ATT_UUID_SIZE] =
{
    CS_STRING_UUID_BASE128(CS_STRING_UUID)
};

// Stream UUID
CONST uint8_t cs_StreamUUID[ATT_UUID_SIZE] =
{
    CS_STREAM_UUID_BASE128(CS_STREAM_UUID)
};

/*********************************************************************
 * LOCAL VARIABLES
 */

static CliServiceCBs_t *pAppCBs = NULL;
static uint8_t cs_icall_rsp_task_id = INVALID_TASK_ID;

/*********************************************************************
 * Profile Attributes - variables
 */

// Service declaration
static CONST gattAttrType_t CliServiceDecl = { ATT_UUID_SIZE, CliServiceUUID };

// Characteristic "String" Properties (for declaration)
static uint8_t cs_StringProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic "String" Value variable
static uint8_t cs_StringVal[CS_STRING_LEN] = {0};

// Length of data in characteristic "String" Value variable, initialized to minimal size.
static uint16_t cs_StringValLen = CS_STRING_LEN_MIN;

// Characteristic "Stream" Properties (for declaration)
static uint8_t cs_StreamProps = GATT_PROP_NOTIFY | GATT_PROP_WRITE_NO_RSP;

// Characteristic "Stream" Value variable
static uint8_t cs_StreamVal[CS_STREAM_LEN] = {0};

// Length of data in characteristic "Stream" Value variable, initialized to minimal size.
static uint16_t cs_StreamValLen = CS_STREAM_LEN_MIN;

// Characteristic "Stream" Client Characteristic Configuration Descriptor
static gattCharCfg_t *cs_StreamConfig;

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t cli_serviceAttrTbl[] =
{
    // cli_service Service Declaration
    {
        { ATT_BT_UUID_SIZE, primaryServiceUUID },
        GATT_PERMIT_READ,
        0,
        (uint8_t *)&CliServiceDecl
    },
    // String Characteristic Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &cs_StringProps
    },
    // String Characteristic Value
    {
        { ATT_UUID_SIZE, cs_StringUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        cs_StringVal
    },
    // Stream Characteristic Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &cs_StreamProps
    },
    // Stream Characteristic Value
    {
        { ATT_UUID_SIZE, cs_StreamUUID },
        GATT_PERMIT_WRITE,
        0,
        cs_StreamVal
    },
    // Stream CCCD
    {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t *)&cs_StreamConfig
    },
};


static bStatus_t cli_service_ReadAttrCB(uint16_t connHandle,
                                         gattAttribute_t *pAttr,
                                         uint8_t *pValue,
                                         uint16_t *pLen,
                                         uint16_t offset,
                                         uint16_t maxLen,
                                         uint8_t method);
static bStatus_t cli_service_WriteAttrCB(uint16_t connHandle,
                                          gattAttribute_t *pAttr,
                                          uint8_t *pValue,
                                          uint16_t len,
                                          uint16_t offset,
                                          uint8_t method);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t cli_serviceCBs =
{
    cli_service_ReadAttrCB, // Read callback function pointer
    cli_service_WriteAttrCB, // Write callback function pointer
    NULL                     // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*
 * CliService_AddService- Initializes the CliService service by registering
 *          GATT attributes with the GATT server.
 *
 *    rspTaskId - The ICall Task Id that should receive responses for Indications.
 */
extern bStatus_t CliService_AddService(uint8_t rspTaskId)
{
    uint8_t status;

    // Allocate Client Characteristic Configuration table
    cs_StreamConfig = (gattCharCfg_t *)ICall_malloc(
        sizeof(gattCharCfg_t) * linkDBNumConns);
    if(cs_StreamConfig == NULL)
    {
        return(bleMemAllocError);
    }

    // Initialize Client Characteristic Configuration attributes
    GATTServApp_InitCharCfg(LINKDB_CONNHANDLE_INVALID, cs_StreamConfig);
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService(cli_serviceAttrTbl,
                                         GATT_NUM_ATTRS(cli_serviceAttrTbl),
                                         GATT_MAX_ENCRYPT_KEY_SIZE,
                                         &cli_serviceCBs);
    Log_info1("Registered service, %d attributes",
              GATT_NUM_ATTRS(cli_serviceAttrTbl));
    cs_icall_rsp_task_id = rspTaskId;

    return(status);
}

/*
 * CliService_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
bStatus_t CliService_RegisterAppCBs(CliServiceCBs_t *appCallbacks)
{
    if(appCallbacks)
    {
        pAppCBs = appCallbacks;
        Log_info1("Registered callbacks to application. Struct %p",
                  (uintptr_t)appCallbacks);
        return(SUCCESS);
    }
    else
    {
        Log_warning0("Null pointer given for app callbacks.");
        return(FAILURE);
    }
}

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
bStatus_t CliService_SetParameter(uint8_t param, uint16_t len, void *value)
{
    bStatus_t ret = SUCCESS;
    uint8_t  *pAttrVal;
    uint16_t *pValLen;
    uint16_t valMinLen;
    uint16_t valMaxLen;
    uint8_t sendNotiInd = FALSE;
    gattCharCfg_t *attrConfig;
    uint8_t needAuth;

    switch(param)
    {
    case CS_STRING_ID:
        pAttrVal = cs_StringVal;
        pValLen = &cs_StringValLen;
        valMinLen = CS_STRING_LEN_MIN;
        valMaxLen = CS_STRING_LEN;
        Log_info2("SetParameter : %s len: %d", (uintptr_t)"String", len);
        break;

    case CS_STREAM_ID:
        pAttrVal = cs_StreamVal;
        pValLen = &cs_StreamValLen;
        valMinLen = CS_STREAM_LEN_MIN;
        valMaxLen = CS_STREAM_LEN;
        sendNotiInd = TRUE;
        attrConfig = cs_StreamConfig;
        needAuth = FALSE;  // Change if authenticated link is required for sending.
        Log_info2("SetParameter : %s len: %d", (uintptr_t)"Stream", len);
        break;

    default:
        Log_error1("SetParameter: Parameter #%d not valid.", param);
        return(INVALIDPARAMETER);
    }

    // Check bounds, update value and send notification or indication if possible.
    if(len <= valMaxLen && len >= valMinLen)
    {
        memcpy(pAttrVal, value, len);
        *pValLen = len; // Update length for read and get.

        if(sendNotiInd)
        {
            Log_info2("Trying to send noti/ind: connHandle %x, %s",
                      attrConfig[0].connHandle,
                      (uintptr_t)((attrConfig[0].value ==
                                   0) ? "\x1b[33mNoti/ind disabled\x1b[0m" :
                                  (attrConfig[0].value ==
                                   1) ? "Notification enabled" :
                                  "Indication enabled"));
            // Try to send notification.
            GATTServApp_ProcessCharCfg(attrConfig, pAttrVal, needAuth,
                                       cli_serviceAttrTbl,
                                       GATT_NUM_ATTRS(
                                           cli_serviceAttrTbl),
                                       cs_icall_rsp_task_id,
                                       cli_service_ReadAttrCB);
        }
    }
    else
    {
        Log_error3("Length outside bounds: Len: %d MinLen: %d MaxLen: %d.", len,
                   valMinLen,
                   valMaxLen);
        ret = bleInvalidRange;
    }

    return(ret);
}

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
bStatus_t CliService_GetParameter(uint8_t param, uint16_t *len, void *value)
{
    bStatus_t ret = SUCCESS;
    switch(param)
    {
    case CS_STRING_ID:
        *len = MIN(*len, cs_StringValLen);
        memcpy(value, cs_StringVal, *len);
        Log_info2("GetParameter : %s returning %d bytes", (uintptr_t)"String",
                  *len);
        break;

    case CS_STREAM_ID:
        *len = MIN(*len, cs_StreamValLen);
        memcpy(value, cs_StreamVal, *len);
        Log_info2("GetParameter : %s returning %d bytes", (uintptr_t)"Stream",
                  *len);
        break;

    default:
        Log_error1("GetParameter: Parameter #%d not valid.", param);
        ret = INVALIDPARAMETER;
        break;
    }
    return(ret);
}

/*********************************************************************
 * @internal
 * @fn          cli_service_findCharParamId
 *
 * @brief       Find the logical param id of an attribute in the service's attr table.
 *
 *              Works only for Characteristic Value attributes and
 *              Client Characteristic Configuration Descriptor attributes.
 *
 * @param       pAttr - pointer to attribute
 *
 * @return      uint8_t paramID (ref cli_service.h) or 0xFF if not found.
 */
static uint8_t cli_service_findCharParamId(gattAttribute_t *pAttr)
{
    // Is this a Client Characteristic Configuration Descriptor?
    if(ATT_BT_UUID_SIZE == pAttr->type.len && GATT_CLIENT_CHAR_CFG_UUID ==
       *(uint16_t *)pAttr->type.uuid)
    {
        return(cli_service_findCharParamId(pAttr - 1)); // Assume the value attribute precedes CCCD and recurse
    }
    // Is this attribute in "String"?
    else if(ATT_UUID_SIZE == pAttr->type.len &&
            !memcmp(pAttr->type.uuid, cs_StringUUID, pAttr->type.len))
    {
        return(CS_STRING_ID);
    }
    // Is this attribute in "Stream"?
    else if(ATT_UUID_SIZE == pAttr->type.len &&
            !memcmp(pAttr->type.uuid, cs_StreamUUID, pAttr->type.len))
    {
        return(CS_STREAM_ID);
    }
    else
    {
        return(0xFF); // Not found. Return invalid.
    }
}

/*********************************************************************
 * @fn          cli_service_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message
 *
 * @return      SUCCESS, blePending or Failure
 */
static bStatus_t cli_service_ReadAttrCB(uint16_t connHandle,
                                         gattAttribute_t *pAttr,
                                         uint8_t *pValue, uint16_t *pLen,
                                         uint16_t offset,
                                         uint16_t maxLen,
                                         uint8_t method)
{
    bStatus_t status = SUCCESS;
    uint16_t valueLen;
    uint8_t paramID = 0xFF;

    // Find settings for the characteristic to be read.
    paramID = cli_service_findCharParamId(pAttr);
    switch(paramID)
    {
    case CS_STRING_ID:
        valueLen = cs_StringValLen;

        Log_info4("ReadAttrCB : %s connHandle: %d offset: %d method: 0x%02x",
                  (uintptr_t)"String",
                  connHandle,
                  offset,
                  method);
        /* Other considerations for String can be inserted here */
        break;

    case CS_STREAM_ID:
        valueLen = cs_StreamValLen;

        Log_info4("ReadAttrCB : %s connHandle: %d offset: %d method: 0x%02x",
                  (uintptr_t)"Stream",
                  connHandle,
                  offset,
                  method);
        /* Other considerations for Stream can be inserted here */
        break;

    default:
        Log_error0("Attribute was not found.");
        return(ATT_ERR_ATTR_NOT_FOUND);
    }
    // Check bounds and return the value
    if(offset > valueLen)   // Prevent malicious ATT ReadBlob offsets.
    {
        Log_error0("An invalid offset was requested.");
        status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
        *pLen = MIN(maxLen, valueLen - offset); // Transmit as much as possible
        memcpy(pValue, pAttr->pValue + offset, *pLen);
    }

    return(status);
}

/*********************************************************************
 * @fn      cli_service_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t cli_service_WriteAttrCB(uint16_t connHandle,
                                          gattAttribute_t *pAttr,
                                          uint8_t *pValue, uint16_t len,
                                          uint16_t offset,
                                          uint8_t method)
{
    bStatus_t status = SUCCESS;
    uint8_t paramID = 0xFF;
    uint8_t changeParamID = 0xFF;
    uint16_t writeLenMin;
    uint16_t writeLenMax;
    uint16_t *pValueLenVar;

    // See if request is regarding a Client Characterisic Configuration
    if(ATT_BT_UUID_SIZE == pAttr->type.len && GATT_CLIENT_CHAR_CFG_UUID ==
       *(uint16_t *)pAttr->type.uuid)
    {
        Log_info3("WriteAttrCB (CCCD): param: %d connHandle: %d %s",
                  cli_service_findCharParamId(pAttr),
                  connHandle,
                  (uintptr_t)(method ==
                              GATT_LOCAL_WRITE ? "- restoring bonded state" :
                              "- OTA write"));

        // Allow notification and indication, but do not check if really allowed per CCCD.
        status = GATTServApp_ProcessCCCWriteReq(
            connHandle, pAttr, pValue, len,
            offset,
            GATT_CLIENT_CFG_NOTIFY |
            GATT_CLIENT_CFG_INDICATE);
        if(SUCCESS == status && pAppCBs && pAppCBs->pfnCfgChangeCb)
        {
            pAppCBs->pfnCfgChangeCb(connHandle,
                                    cli_service_findCharParamId(
                                        pAttr), len, pValue);
        }

        return(status);
    }

    // Find settings for the characteristic to be written.
    paramID = cli_service_findCharParamId(pAttr);
    switch(paramID)
    {
    case CS_STRING_ID:
        writeLenMin = CS_STRING_LEN_MIN;
        writeLenMax = CS_STRING_LEN;
        pValueLenVar = &cs_StringValLen;

        Log_info5(
            "WriteAttrCB : %s connHandle(%d) len(%d) offset(%d) method(0x%02x)",
            (uintptr_t)"String",
            connHandle,
            len,
            offset,
            method);
        /* Other considerations for String can be inserted here */
        break;

    case CS_STREAM_ID:
        writeLenMin = CS_STREAM_LEN_MIN;
        writeLenMax = CS_STREAM_LEN;
        pValueLenVar = &cs_StreamValLen;

        Log_info5(
            "WriteAttrCB : %s connHandle(%d) len(%d) offset(%d) method(0x%02x)",
            (uintptr_t)"Stream",
            connHandle,
            len,
            offset,
            method);
        /* Other considerations for Stream can be inserted here */
        break;

    default:
        Log_error0("Attribute was not found.");
        return(ATT_ERR_ATTR_NOT_FOUND);
    }
    // Check whether the length is within bounds.
    if(offset >= writeLenMax)
    {
        Log_error0("An invalid offset was requested.");
        status = ATT_ERR_INVALID_OFFSET;
    }
    else if(offset + len > writeLenMax)
    {
        Log_error0("Invalid value length was received.");
        status = ATT_ERR_INVALID_VALUE_SIZE;
    }
    else if(offset + len < writeLenMin &&
            (method == ATT_EXECUTE_WRITE_REQ || method == ATT_WRITE_REQ))
    {
        // Refuse writes that are lower than minimum.
        // Note: Cannot determine if a Reliable Write (to several chars) is finished, so those will
        //       only be refused if this attribute is the last in the queue (method is execute).
        //       Otherwise, reliable writes are accepted and parsed piecemeal.
        Log_error0("Invalid value length was received.");
        status = ATT_ERR_INVALID_VALUE_SIZE;
    }
    else
    {
        // Copy pValue into the variable we point to from the attribute table.
        memcpy(pAttr->pValue + offset, pValue, len);

        // Only notify application and update length if enough data is written.
        //
        // Note: If reliable writes are used (meaning several attributes are written to using ATT PrepareWrite),
        //       the application will get a callback for every write with an offset + len larger than _LEN_MIN.
        // Note: For Long Writes (ATT Prepare + Execute towards only one attribute) only one callback will be issued,
        //       because the write fragments are concatenated before being sent here.
        if(offset + len >= writeLenMin)
        {
            changeParamID = paramID;
            *pValueLenVar = offset + len; // Update data length.
        }
    }

    // Let the application know something changed (if it did) by using the
    // callback it registered earlier (if it did).
    if(changeParamID != 0xFF)
    {
        if(pAppCBs && pAppCBs->pfnChangeCb)
        {
            pAppCBs->pfnChangeCb(connHandle, paramID, len + offset, pValue); // Call app function from stack task context.
        }
    }
    return(status);
}
