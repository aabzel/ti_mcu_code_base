#ifndef UBX_PROTOCOL_H
#define UBX_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define UBX_SYN_0 0xB5 /* u */
#define UBX_SYN_1 0x62 /* b */

#define UBX_INDEX_CLS 2U
#define UBX_INDEX_ID 3U
#define UBX_INDEX_LEN 4U
#define UBX_INDEX_PAYLOAD 6U

#define UBX_RX_FRAME_SIZE 100U
#define UBX_LEN_SIZE 2
#define UBX_CRC_SIZE 2
#define UBX_HEADER_SIZE (UBX_LEN_SIZE + 4)

#define UBX_ACK_ACK 0x01
#define UBX_ACK_NAK 0x00

/*UBX messages overview*/
#define UBX_CLA_NAV 0x01
#define UBX_CLA_RXM 0x02
#define UBX_CLA_INF 0x04
#define UBX_CLA_ACK 0x05
#define UBX_CLA_CFG 0x06
#define UBX_CLA_UPD 0x09
#define UBX_CLA_MON 0x0A
#define UBX_CLA_TIM 0x0D
#define UBX_CLA_ESF 0x10
#define UBX_CLA_MGA 0x13
#define UBX_CLA_SEC 0x27

/*3.10.26 UBX-CFG-VALSET (0x06 0x8a)*/
#define UBX_ID_CFG_SET_VAL 0x8A
/*3.10.25 UBX-CFG-VALGET (0x06 0x8b)*/
#define UBX_ID_CFG_GET_VAL 0x8B
/*3.10.21 (0x06 0x71)*/
#define UBX_ID_CFG_TMODE3 0x71
/*3.10.2.1 Clear, save and load configurations*/
#define UBX_ID_CFG_CFG 0x09
/*3.10.17 UBX-CFG-RATE (0x06 0x08)*/
#define UBX_ID_CFG_RATE 0x08

#define UBX_ID_NAV_POSLLH 0x02
#define UBX_ID_NAV_STATUS 0x03
#define UBX_ID_NAV_ATT 0x05
#define UBX_ID_NAV_VELECEF 0x11
#define UBX_ID_NAV_VELNED 0x12
#define UBX_ID_NAV_HPPOSLLH 0x14
#define UBX_ID_NAV_TIMEUTC 0x21
#define UBX_ID_NAV_CLK 0x22

#define UBX_ID_MON_VER 0x04
#define UBX_ID_MON_HW 0x09

#define UBX_ID_SEC_UNIQID 0x03

#define UBX_CLA_CNT 15

typedef enum eUbxProtState_t {
    UBX_WAIT_SYC0 = 0U,
    UBX_WAIT_SYC1 = 1U,
    UBX_WAIT_CLASS = 2U,
    UBX_WAIT_ID = 3U,
    UBX_WAIT_LEN = 4U,
    UBX_WAIT_PAYLOAD = 5U,
    UBX_WAIT_CRC = 6U,
    UBX_RX_DONE = 7U
} UbxProtState_t;

#define LAYER_MASK_RAM (1)
#define LAYER_MASK_BBR (2) /*Battery-backed RAM*/
#define LAYER_MASK_FLASH (4)
#define LAYER_MASK_RAM_FLASH (LAYER_MASK_RAM & LAYER_MASK_FLASH)

typedef struct xLayers_t {
    union {
        uint8_t byte;
        struct {
            uint8_t ram : 1;   /* Update configuration in the RAM layer*/
            uint8_t bbr : 1;   /* Update configuration in the BBR layer Battery-backed RAM*/
            uint8_t flash : 1; /* Update configuration in the Flash layer*/
            uint8_t rsrv : 5;
        };
    };
} Layers_t;

/*3.10.21 UBX-CFG-TMODE3 (0x06 0x71)*/
typedef struct xUbxCfgTmode3Data_t {
    uint8_t version;       /*0  Message version (0x00 for this version)*/
    uint8_t reserved0;     /*1  Reserved*/
    uint8_t mode;          /*2  Receiver Mode:*/
    uint8_t lla;           /*3  Position is given in LAT/LON/ALT (default is ECEF)*/
    int32_t ecefXOrLat;    /*4  WGS84 ECEF X coordinate (or latitude) of the ARP position, depending on flags above*/
    int32_t ecefYOrLon;    /*8  WGS84 ECEF Y coordinate (or longitude) of the ARP position, depending on flags above*/
    int32_t ecefZOrAlt;    /*12 WGS84 ECEF Z coordinate (or altitude) of the ARP position, depending on flags above*/
    int8_t ecefXOrLatHP;   /*16 High-precision WGS84 ECEF X coordinate (or latitude) of the ARP position, depending on
                              flags above.*/
    int8_t ecefYOrLonHP;   /*17 High-precision WGS84 ECEF Y coordinate (or longitude) of the ARP position, depending on
                              flags above.*/
    int8_t ecefZOrAltHP;   /*18 High-precision WGS84 ECEF Z coordinate (or altitude) of the ARP position, depending on
                              flags above. Must be in the range -99..+99.*/
    uint8_t reserved1;     /*19 Fixed position 3D accuracy*/
    uint32_t fixedPosAcc;  /*20 Fixed position 3D accuracy*/
    uint32_t svin_min_dur_s;   /*24 Survey-in minimum duration in seconds*/
    uint32_t svinAccLimit; /*28 Survey-in position accuracy limit*/
    uint8_t reserved2[8];  /*32 Fixed position 3D accuracy*/
} UbxCfgTmode3Data_t_t;

/*3.10.2 UBX-CFG-CFG (0x06 0x09)*/
typedef struct xUbxCfgCfg_t {
    uint32_t clearMask; /*Mask for configuration to clear*/
    uint32_t saveMask;  /*Mask for configuration to save*/
    uint32_t loadMask;  /*Mask for configuration to load*/
    uint8_t deviceMask; /*deviceMask*/
} __attribute__((__packed__)) UbxCfgCfg_t;

/*3.10.17 UBX-CFG-RATE (0x06 0x08)*/
typedef struct xUbxCfgRate_t {
    uint16_t meas_rate_ms; /*0 ms The elapsed time between GNSS measurements*/
    uint16_t navRate; /*2 cycles The ratio between the number of measurements and the number of navigation solutions*/
    uint16_t timeRef; /*4 */
} __attribute__((__packed__)) UbxCfgRate_t;

typedef struct xConfigurationKeyID_t {
    union {
        uint32_t word;
        struct {
            uint16_t
                id_within_group : 12; /* Twelve bits that define a unique item ID within a group (range 0x001-0xffe)*/
            uint16_t rsvd1 : 4;       /*Currently unused. Reserved for future use.*/
            uint16_t group : 8;       /*Eight bits that define a unique group ID (range 0x01-0xfe)*/
            uint16_t rsvd2 : 4;       /*Currently unused. Reserved for future use.*/
            uint16_t size : 3;  /*Three bits that indicate the storage size of a Configuration Value (range 0x01-0x05,*/
            uint16_t rsvd3 : 1; /*Currently unused. Reserved for future use.*/
        };
    };
} __attribute__((__packed__)) ConfigurationKeyID_t;

typedef struct xCfgValGetHeader_t {
    uint8_t version;
    Layers_t layer;
    uint16_t position;
} CfgValGetHeader_t;

typedef struct xUbloxProtocol_t {
    uint32_t rx_time_stamp;
    uint32_t rx_pkt_cnt;
    uint32_t tx_pkt_cnt;
    uint32_t crc_err_cnt;
    uint32_t load_len;
    uint16_t exp_len;
#ifdef HAS_DEBUG
    uint16_t min_len;
    uint16_t max_len;
    uint32_t sync_cnt;
#endif
    uint16_t read_crc;
    volatile uint32_t ack_cnt;
    volatile bool ack;
    uint8_t rx_state;
    uint8_t rx_frame[UBX_RX_FRAME_SIZE];
    uint8_t fix_frame[UBX_RX_FRAME_SIZE];
    bool unproc_frame;
    bool diag;
} UbloxProtocol_t;

typedef struct xUbxHeader_t {
    uint8_t class;
    uint8_t id;
} UbxHeader_t;

typedef struct xStatClass_t {
    uint8_t class_val;
    uint16_t cnt;
} StatClass_t;

extern UbloxProtocol_t UbloxProtocol;
extern StatClass_t tableRxClass[UBX_CLA_CNT];

bool ubx_reset_rx(void);
bool ublox_protocol_init(void);
bool ubx_proc_byte(uint8_t rx_byte);
bool ubx_print_frame(uint8_t* frame);
bool ubx_update_stat(uint8_t val_class);
bool ubx_print_frame(uint8_t* frame);
uint16_t ubx_calc_crc16(uint8_t* array, uint16_t len);
uint8_t ubx_key_len_2bytes(uint8_t code);
uint8_t ubx_keyid_2len(uint32_t key_id);
bool ubx_wait_ack(uint32_t wait_pause_ms);
#ifdef __cplusplus
}
#endif

#endif /* UBX_PROTOCOL_H */
