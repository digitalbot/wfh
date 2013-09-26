/*
 * wav_file_handle.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav_file_handle.h"


FILE *open_wav(char *filename, OpenFlags flag) {
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

int close_wav(FILE *fp) {
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] something is wrong! file pointer is NULL!\n");
        return EXIT_FAILURE;
    }
    int temp = fclose(fp);
    return temp;
}

int read_riff_chunk(FILE *fp, RiffChunk *riff_chunk) {
    if (ftell(fp)) {
        long offset = 0;
        fseek(fp, offset, SEEK_SET);
    }

    RiffChunk temp;
    fread(&temp, sizeof(temp), 1, fp);
    riff_chunk = &temp;
    if (memcmp(riff_chunk->file_type, "RIFF", 4)) {
        fprintf(stderr, "[ERROR] This is NOT 'RIFF' file.\n");
        return EXIT_FAILURE;
    }
    if (memcmp(riff_chunk->riff_type, "WAVE", 4)) {
        fprintf(stderr, "[ERROR] This is NOT 'WAVE' file.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
