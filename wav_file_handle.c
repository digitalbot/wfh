/*
 * wav_file_handle.c
 */


#include <stdio.h>
#include <string.h>

#include "wav_file_handle.h"


FILE *OpenWav(char *filename, OpenFlags flag) {
    FILE *temp;
    switch (flag) {
    case kIsRead:
        temp = fopen(filename, "rb");
        break;
    case kIsWrite:
        temp = fopen(filename, "wb");
        break;
    }
    if (temp == NULL) {
        fprintf(stderr, "[ERROR] Can't open %s.\n", filename);
        return NULL;
    }
    return temp;
}

int CloseWav(FILE *fp) {
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] something is wrong!\n");
        return -1;
    }
    fclose(fp);
    return 0;
}

int ReadRiffChunk(FILE *fp, RiffChunk *riff_chunk) {
    RiffChunk temp;
    fread(&temp, sizeof(temp), 1, fp);
    riff_chunk = &temp;
    if (memcmp(riff_chunk->file_type, "RIFF", 4)) {
        fprintf(stderr, "[ERROR] This is NOT 'RIFF' file.\n");
        return -1;
    }
    if (memcmp(riff_chunk->riff_type, "WAVE", 4)) {
        fprintf(stderr, "[ERROR] This is NOT 'WAVE' file.\n");
        return -1;
    }
    return 0;
}
