/* mbr.c - Accès aux périphériques */
/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/
#include "mbr.h"
#include <assert.h>
#include <stdlib.h>
#include "drive.h"
#include "constantes.h"


void load_mbr() {
    assert(sizeof(struct mbr_s) <= tailleSec);
    read_sector_n(0, 0, &mbr, sizeof(struct mbr_s));
    if (mbr.magic != MAGIC) {
        mbr.magic = MAGIC;
        mbr.nb_volumes = 0;
    }
}

void save_mbr() {
	assert(mbr.magic == MAGIC);
	write_sector_n(0, 0, &mbr, sizeof(mbr));
}
