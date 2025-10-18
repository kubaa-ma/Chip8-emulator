#pragma once

#include <stdbool.h>
#include <stdint.h>

/* Most cards require pretty long delay.
 * You can adjust to yours, but it doesn't matter that much in the end,
 * because regardless of the delay, CMD0 must be sent several times.
 * Just keep it >= 1 ms.
 */
#define SDCARD_BOOT_DELAY 1000 /* ms */

typedef enum SdVer {
    SD_VER_1,
    SD_VER_2
} SdVer;

typedef enum SdType {
    SD_TYPE_SDSC,
    SD_TYPE_SDHC
} SdType;

typedef enum SdVoltRange {
    SD_VOLT_UNKNOWN = 0x00,
    SD_VOLT_2_7_TO_3_6 = 0x01,
    SD_VOLT_LOW = 0x02,
    SD_VOLT_RESERVED1 = 0x04,
    SD_VOLT_RESERVED2 = 0x08,
} SdVoltRange;

typedef struct SdCard {
    SdVer ver;
    SdType type;
    SdVoltRange volt_range;
} SdCard;

bool sdcard_init(SdCard *sd);
bool sdcard_read_block(SdCard *sd, uint32_t addr, uint8_t *buf);
bool sdcard_write_block(SdCard *sd, uint32_t addr, const uint8_t *data);
