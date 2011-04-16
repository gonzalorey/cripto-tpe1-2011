/*
 * wavParser.c
 *
 *  Created on: Apr 16, 2011
 */

#include <stdio.h>
#include "wav.h"
#include "debug.h"

int parseRiff (CKID	chunkID, CKSIZE	chunkSize, CKID	format);
int parseFMK (CKID	chunkID, CKSIZE	chunkSize, WORD	wFormatTag, WORD wChannels, WORD wBitsPerSample);
int parseData (CKID	chunkID);

int wavParser (FILE * source) {
	RIFF_CK riff;
	FMT_CK fmt;
	DATA_CK data;
	int bytes;
	if ((bytes = fread(&riff, sizeof(RIFF_CK), 1, source)) == 0){
		FATAL("Cantidad de bytes leidos: %d\n", bytes);
		return 0;
	}
	if (!parseRiff(riff.chunkID, riff.chunkSize, riff.format)) {
		return 0;
	}
	if ((bytes = fread(&fmt, sizeof(FMT_CK), 1, source)) == 0){
		FATAL("Cantidad de bytes leidos: %d\n", bytes);
		return 0;
	}
	if (!parseFMK(fmt.chunkID, fmt.chunkSize, fmt.wFormatTag, fmt.wChannels, fmt.wBitsPerSample)) {
		return 0;
	}
	if ((bytes = fread(&data, sizeof(CKID) + sizeof(CKSIZE), 1, source)) == 0){
		FATAL("Cantidad de bytes leidos: %d\n", bytes);
		return 0;
	}
	if ((bytes = fread(&(data.soundData), data.chunkSize, 1, source)) == 0){
		FATAL("Cantidad de bytes leidos: %d\n", bytes);
		return 0;
	}
	if (!parseData(data.chunkID)) {
		return 0;
	}
	return 1;
}

int parseRiff (CKID	chunkID, CKSIZE	chunkSize, CKID	format) {
	if (chunkID != RIFF_CKID || format != RIFF_FORMAT) {
		return 0;
	}
	//TODO
//	if (chunkSize != ?????) {
//		return 0;
//	}
	return 1;
}

int parseFMK (CKID	chunkID, CKSIZE	chunkSize, WORD	wFormatTag, WORD wChannels, WORD wBitsPerSample) {
	if (chunkID != FMT_CKID || chunkSize != FMT_CKSIZE || wFormatTag != FMT_FORMAT_CAT ||
			wChannels != CHANNEL_MONO) {
		return 0;
	}
	if (wBitsPerSample % 8 != 0) {
		return 0;
	}
	return 1;
}

int parseData (CKID	chunkID) {
	if (chunkID != DATA_CKID) {
		return 0;
	}
	return 1;
}
