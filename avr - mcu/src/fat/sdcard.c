#include "sdcard.h"

#include "spi.h"

#include <avr/io.h>
#include <util/delay.h>

#define card_select()                                                          \
    spi_transfer(0xff);                                                        \
    spi_slave_select();                                                        \
    spi_transfer(0xff)

#define card_deselect()                                                        \
    spi_transfer(0xff);                                                        \
    spi_slave_deselect();                                                      \
    spi_transfer(0xff)

typedef enum R1Bit {
    R1_IDLE = (1u << 0),
    R1_ERASE_RESET = (1u << 1),
    R1_ILLEGAL = (1u << 2),
    R1_CRC_ERR = (1u << 3),
    R1_ERASE_SEQ_ERR = (1u << 4),
    R1_ADDR_ERR = (1u << 5),
    R1_PARAM_ERR = (1u << 6)
} R1Bit;

typedef enum CmdIdx {
    CMD_RESET = 0,
    CMD_INTERFACE_COND = 8,
    CMD_BLOCK_SIZE = 16,
    CMD_READ_BLOCK = 17,
    CMD_WRITE_BLOCK = 24,
    CMD_APP_INIT = 41,
    CMD_APP = 55,
    CMD_OCR = 58,
} CmdIdx;

static void send_cmd(CmdIdx idx, uint32_t arg, uint8_t crc) {
    spi_transfer((uint8_t) idx | (1 << 6));
    spi_transfer((uint8_t) (arg >> 24) & 0xff);
    spi_transfer((uint8_t) (arg >> 16) & 0xff);
    spi_transfer((uint8_t) (arg >> 8) & 0xff);
    spi_transfer((uint8_t) (arg));
    spi_transfer(crc | 0x01);
}

static uint8_t read_r1(void) {
    uint8_t r1 = 0;

    for (uint8_t i = 0; i <= 8; ++i) {
        r1 = spi_transfer(0xff);

        if (r1 != 0xff) {
            break;
        }
    }

    return r1;
}

void power_up_seq(void) {
    spi_slave_deselect();

    /* let the SD card to boot up */
    _delay_ms(SDCARD_BOOT_DELAY);

    /* dummy 160 cycles (8 bits * 20 = 160) */
    for (uint8_t i = 0; i < 20; ++i) {
        spi_transfer(0xff);
    }

    /* software reset */
    spi_slave_deselect();
    spi_transfer(0xff);
}

uint8_t enter_idle_state(void) {
    card_select();

    send_cmd(CMD_RESET, 0x00, 0x95);
    uint8_t r1 = read_r1();

    card_deselect();

    return r1;
}

void read_r7(uint8_t *r1, uint32_t *ocr) {
    *r1 = read_r1();

    if (*r1 > 1) {
        return;
    }

    uint8_t b1 = spi_transfer(0xff);
    uint8_t b2 = spi_transfer(0xff);
    uint8_t b3 = spi_transfer(0xff);
    uint8_t b4 = spi_transfer(0xff);

    *ocr = 0x00;
    *ocr |= (uint32_t) b1 << 24;
    *ocr |= (uint32_t) b2 << 16;
    *ocr |= (uint32_t) b3 << 8;
    *ocr |= b4;
}

bool get_interface_cond(uint8_t *r1, SdCard *sd) {
    card_select();

    send_cmd(CMD_INTERFACE_COND, 0x000001aa, 0x86);
    uint32_t ocr;
    read_r7(r1, &ocr);

    if (*r1 & R1_ILLEGAL) {
        sd->ver = SD_VER_1;
        sd->volt_range = SD_VOLT_UNKNOWN;

        return true;
    }

    card_deselect();

    uint8_t check_pattern = ocr & 0xff;
    uint8_t volt_range = (ocr >> 8) & 0xff;

    if (check_pattern != 0xaa) {
        return false;
    }

    sd->ver = SD_VER_2;
    sd->volt_range = (SdVoltRange) volt_range;

    return true;
}

uint8_t send_app_prefix(void) {
    card_select();

    send_cmd(CMD_APP, 0x00, 0x65);
    uint8_t r1 = read_r1();

    card_deselect();

    return r1;
}

uint8_t send_app_init(void) {
    card_select();

    send_cmd(CMD_APP_INIT, 0x40000000, 0x77);
    uint8_t r1 = read_r1();

    card_deselect();

    return r1;
}

uint8_t send_block_size(uint32_t size) {
    card_select();

    send_cmd(CMD_BLOCK_SIZE, size, 0x00);
    uint8_t r1 = read_r1();

    card_deselect();

    return r1;
}

void read_ocr(uint8_t *r1, uint32_t *ocr) {
    card_select();

    send_cmd(CMD_OCR, 0x00, 0x00);
    read_r7(r1, ocr);

    card_deselect();
}

typedef enum IoResult {
    IO_OK,
    IO_ERROR,
    IO_TIMEOUT
} IoResult;

bool sdcard_init(SdCard *sd) {
    power_up_seq();

    uint8_t r1 = enter_idle_state();
    if (r1 != R1_IDLE) {
        for (uint8_t i = 0; i < 5; ++i) {
            r1 = enter_idle_state();

            if (r1 != 0x3f && r1 != 0xff) {
                break;
            }
        }

        if (r1 != R1_IDLE) {
            return false;
        }
    }

    if (!get_interface_cond(&r1, sd)) {
        return false;
    }

    if (r1 != R1_IDLE && !(r1 & R1_ILLEGAL)) {
        return false;
    }

    uint8_t attempt_cnt = 0;
    do {
        if (attempt_cnt >= 0xff) {
            return false;
        }

        r1 = send_app_prefix();

        if (r1 < 0x02) {
            r1 = send_app_init();
        }

        _delay_ms(10);

        ++attempt_cnt;
    } while (r1 != 0x00);

    uint32_t ocr;
    read_ocr(&r1, &ocr);

    if (r1 != 0x00) {
        return false;
    }

    if (!(ocr & 0x80000000)) {
        return false;
    }

    if (ocr & 0x40000000) {
        sd->type = SD_TYPE_SDHC;
    } else {
        sd->type = SD_TYPE_SDSC;

        r1 = send_block_size(512);

        if (r1 != 0x00) {
            return false;
        }
    }

    return true;
}

bool sdcard_read_block(SdCard *sd, uint32_t addr, uint8_t *buf) {
    card_select();

    if (sd->type == SD_TYPE_SDSC) {
        addr *= 512;
    }

    send_cmd(CMD_READ_BLOCK, addr, 0x00);
    uint8_t r1 = read_r1();

    if (r1 > 1) {
        card_deselect();

        return false;
    }

    uint8_t token = 0xff;

    for (uint16_t i = 0; i < 10000; ++i) {
        token = spi_transfer(0xff);

        if (token != 0xff) {
            break;
        }
    }

    if (token == 0xfe) {
        for (uint32_t i = 0; i < 512; ++i) {
            buf[i] = spi_transfer(0xff);
        }

        /* skip OCR */
        spi_transfer(0xff);
        spi_transfer(0xff);
    }

    card_deselect();

    return true;
}

bool sdcard_write_block(SdCard *sd, uint32_t addr, const uint8_t *data) {
    card_select();

    if (sd->type == SD_TYPE_SDSC) {
        addr *= 512;
    }

    send_cmd(CMD_WRITE_BLOCK, addr, 0x00);
    uint8_t r1 = read_r1();

    if (r1 > 1) {
        card_deselect();

        return false;
    }

    uint8_t token = 0xff;

    spi_transfer(0xfe);

    for (uint32_t i = 0; i < 512; ++i) {
        spi_transfer(data[i]);
    }

    for (uint16_t i = 0; i < 10000; ++i) {
        token = spi_transfer(0xff);

        if (token != 0xff) {
            break;
        }
    }

    if ((token & 0x1f) == 0x05) {
        for (uint16_t i = 0; i < 10000; ++i) {
            token = spi_transfer(0xff);

            if (token != 0xff) {
                break;
            }
        }
    }

    card_deselect();

    return true;
}
