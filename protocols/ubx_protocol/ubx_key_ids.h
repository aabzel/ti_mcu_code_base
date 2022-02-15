#ifndef UBX_KEY_IDS_H
#define UBX_KEY_IDS_H
#define CFG_MSGOUT_UBX_NAV_PVT_UART1   0x20910007
#define CFG_MSGOUT_UBX_NAV_SVIN_UART1  0x20910089   //U1
#define CFG_MSGOUT_UBX_NAV_HPPOSLLH_UART1 0x20910034
#define CFG_MSGOUT_UBX_NAV_POSLLH_UART1 0x2091002a
#define CFG_MSGOUT_UBX_NAV_VELNED_UART1 0x20910043
#define CFG_MSGOUT_UBX_NAV_TIMEUTC_UART1 0x2091005c
#define CFG_MSGOUT_NMEA_ID_ZDA_UART1 0x209100d9 //U1 - - Output rate of the NMEA-GX-ZDA message on port UART1
#define CFG_MSGOUT_NMEA_ID_GSA_UART1 0x209100c0

#ifdef HAS_GPS_CORRECTION
#define CFG_MSGOUT_RTCM_3X_TYPE1074_UART1 0x2091035f // GPS MSM4 (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1077_UART1 0x209102cd // GPS MSM7 (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1074_USB 0x20910361   // GPS MSM4 (Input/output)
#endif

#define CFG_MSGOUT_RTCM_3X_TYPE1005_UART1 0x209102be // Stationary RTK reference station ARP (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1005_USB 0x209102c0   // Stationary RTK reference station ARP (Input/output)
#ifdef HAS_GLONASS_CORRECTION
#define CFG_MSGOUT_RTCM_3X_TYPE1084_UART1 0x20910364 // GLONASS MSM4 (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1084_USB 0x20910366   // GLONASS MSM4 (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1087_UART1 0x209102d2 // GLONASS MSM7 (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1230_UART1 0x20910304 // GLONASS L1 and L2 code-phase biases (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1230_USB 0x20910306   // GLONASS L1 and L2 code-phase biases (Input/output)
#endif
#ifdef HAS_GALILEO_CORRECTION
#define CFG_MSGOUT_RTCM_3X_TYPE1094_UART1 0x20910369 // Galileo MSM4 (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1094_USB 0x2091036b   // Galileo MSM4 (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1097_UART1 0x20910319 // Galileo MSM7 (Input/output)
#endif
#ifdef HAS_BEI_DOU_CORRECTION
#define CFG_MSGOUT_RTCM_3X_TYPE1124_UART1 0x2091036e // BeiDou MSM4 (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1124_USB 0x20910370   // BeiDou MSM4 (Input/output)
#define CFG_MSGOUT_RTCM_3X_TYPE1127_UART1 0x209102d7 // BeiDou MSM7 (Input/output)
#endif
#define CFG_MSGOUT_UBX_NAV_PVT_USB 0x20910009
#define CFG_MSGOUT_UBX_NAV_SVIN_USB 0x2091008b
#define CFG_UART1_BAUDRATE 0x40520001
#define CFG_UART1INPROT_NMEA 0x10730002
#define CFG_UART1INPROT_RTCM3X 0x10730004
#define CFG_UART1INPROT_UBX 0x10730001
#define CFG_UART1OUTPROT_NMEA 0x10740002
#define CFG_UART1OUTPROT_RTCM3X 0x10740004
#define CFG_UART1OUTPROT_RTCM3X 0x10740004
#define CFG_UART1OUTPROT_UBX 0x10740001
#define CFG_USBINPROT_NMEA 0x10770002
#define CFG_USBINPROT_RTCM3X 0x10770004
#define CFG_USBINPROT_UBX 0x10770001
#define CFG_USBOUTPROT_RTCM3X 0x10780004
#define CFG_USBOUTPROT_UBX 0x10780001

#endif /* UBX_KEY_IDS_H */
