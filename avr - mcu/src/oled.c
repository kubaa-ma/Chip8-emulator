#include "oled.h"

void TWI_init(){
    TWSR = 0x00;
    TWBR = 72;
    TWCR = (1<<TWEN);
}

void TWI_start(){
    TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
    while(!(TWCR & (1<<TWINT)));
}

void TWI_stop(){
    TWCR = (1<<TWSTO)|(1<<TWEN);
}

void TWI_write(uint8_t data){
    TWDR = data;
    TWCR = (1<<TWEN)|(1<<TWINT);
    while(!(TWCR & (1<<TWINT)));
}

uint8_t TWI_read_ACK(){
    TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWEA);
    while(!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t TWI_read_NACK(){
    TWCR = (1<<TWEN)|(1<<TWINT);
    while(!(TWCR & (1<<TWINT)));
    return TWDR;
}

static void Oled_command(uint8_t command){
    TWI_start();
    TWI_write((SLAVE_ADDR<<1)|0);
    TWI_write(0x00);
    TWI_write(command);
    TWI_stop();
}

void Oled_send_commands(const uint8_t *cmds, uint8_t len) {
    TWI_start();
    TWI_write((SLAVE_ADDR<<1)|0);
    TWI_write(0x00);
    for(uint8_t i=0;i<len;i++)
        TWI_write(cmds[i]);
    TWI_stop();
}

static void Oled_set_position(uint8_t page, uint8_t column) {
    Oled_command(0xB0|page);
    Oled_command(0x00|(column&0x0F));
    Oled_command(0x10|(column>>4));
}

void Oled_init(){
    const uint8_t init_data[] = {
        0xAE,0xD5,0x80,0xA8,0x3F,0xD3,0x00,0x40,0x8D,0x14,
        0x20,0x00,0xA1,0xC8,0xDA,0x12,0x81,0x7F,0xD9,0xF1,
        0xDB,0x40,0xA4,0xA6,0x2E,0xAF
    };
    Oled_send_commands(init_data,sizeof(init_data));
}

void Oled_draw_buffer(uint8_t *buff) {
    for(uint8_t page=0;page<8;page++) {
        Oled_set_position(page,0);
        TWI_start();
        TWI_write((SLAVE_ADDR<<1)|0);
        TWI_write(0x40);
        for(uint8_t col=0;col<128;col++)
            TWI_write(buff[page*128+col]);
        TWI_stop();
    }
}
