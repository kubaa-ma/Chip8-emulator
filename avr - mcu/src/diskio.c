#include "diskio.h"
#include "storage.h"
#include "ff.h"
#include <stdint.h>

DSTATUS disk_status(BYTE pdrv) {		// Skiped due to using only SD CARD
    (void)pdrv;
    return 0; 
}

DSTATUS disk_initialize(BYTE pdrv) {	// Skiped due to using only SD CARD
    (void)pdrv;
    SPI_init();
    
    return 0;
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count) {	// Skiped due to using only SD CARD
    
    return RES_OK;
}

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count) {
    (void)pdrv; (void)buff; (void)sector; (void)count;
    return RES_OK;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff) {
    (void)pdrv; (void)cmd; (void)buff;
    return RES_OK;
}
