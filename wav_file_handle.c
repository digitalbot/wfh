/*
 * wav_file_handle.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
    return fclose(fp);
}

int read_riff_chunk(FILE *fp, RiffChunk *chunk) {
    if (ftell(fp)) {
        long offset = 0;
        fseek(fp, offset, SEEK_SET);
    }
    RiffChunk temp;
    fread(&temp, sizeof(temp), 1, fp);
    if (memcmp(temp.file_type, "RIFF", 4)) {
        fprintf(stderr, "[ERROR] This is NOT 'RIFF' file.\n");
        return EXIT_FAILURE;
    }
    if (temp.size_of_riff <= 0) {
        fprintf(stderr, "[ERROR] This is empty file.\n");
        return EXIT_FAILURE;
    }
    if (memcmp(temp.riff_type, "WAVE", 4)) {
        fprintf(stderr, "[ERROR] This is NOT 'WAVE' file.\n");
        return EXIT_FAILURE;
    }

    *chunk = temp;
    return EXIT_SUCCESS;
}

int read_fmt_chunk(FILE *fp, FmtChunk *chunk) {
    if (ftell(fp) != sizeof(RiffChunk)) {
        long offset = sizeof(RiffChunk);
        fseek(fp, offset, SEEK_SET);
    }

    FmtChunk temp;
    fread(&temp, sizeof(temp), 1, fp);
    if (memcmp(temp.chunk_type, "fmt ", 4)) {
        fprintf(stderr, "[ERROR] This is NOT 'fmt ' file.\n");
        return EXIT_FAILURE;
    }
    if (temp.size_of_fmt <= 0) {
        fprintf(stderr, "[ERROR] 'fmt ' chunk is empty.\n");
        return EXIT_FAILURE;
    }
    if (temp.format_tag <= 0) {
        fprintf(stderr, "[ERROR] format tag is wrong.\n");
        return EXIT_FAILURE;
    }
    if (temp.num_of_channels <= 0) {
        fprintf(stderr, "[ERROR] number of channels is wrong.\n");
        return EXIT_FAILURE;
    }
    if (temp.samples_per_sec <= 0) {
        fprintf(stderr, "[ERROR] samples per second is wrong.\n");
        return EXIT_FAILURE;
    }
    if (! (temp.bits_per_sample == sizeof(char) * CHAR_BIT ||
           temp.bits_per_sample == sizeof(short) * CHAR_BIT ||
           temp.bits_per_sample == sizeof(int) * CHAR_BIT ||
           temp.bits_per_sample == sizeof(float) * CHAR_BIT ||
           temp.bits_per_sample == sizeof(double) * CHAR_BIT)) {
        fprintf(stderr, "[ERROR] bits per sample is wrong.\n");
        return EXIT_FAILURE;
    }
    if (temp.size_of_block != temp.bits_per_sample / CHAR_BIT * temp.num_of_channels) {
        fprintf(stderr, "[ERROR] size of block is wrong.\n");
        return EXIT_FAILURE;
    }
    if (temp.bytes_per_sec != temp.samples_per_sec * temp.size_of_block) {
        fprintf(stderr, "[ERROR] bytes per second is wrong.\n");
        return EXIT_FAILURE;
    }

    *chunk = temp;
    return EXIT_SUCCESS;
}

int read_data_chunk(FILE *fp, DataChunk *chunk) {
    if (ftell(fp) != sizeof(RiffChunk) + sizeof(FmtChunk)) {
        long offset = sizeof(RiffChunk) + sizeof(FmtChunk);
        fseek(fp, offset, SEEK_SET);
    }

    DataChunk temp;
    fread(&temp.chunk_type, sizeof(temp.chunk_type), 1, fp);
    if (memcmp(temp.chunk_type, "data", 4)) {
        fseek(fp, - sizeof(temp.chunk_type), SEEK_CUR);

        unsigned short size_of_ext = 0;
        fread(&size_of_ext, sizeof(size_of_ext), 1, fp);
        fseek(fp, size_of_ext, SEEK_CUR);
        fread(&temp.chunk_type, sizeof(temp.chunk_type), 1, fp);
        if (memcmp(temp.chunk_type, "data", 4)) {
            fprintf(stderr, "[ERROR] This file has not 'data' chunk.\n");
            return EXIT_FAILURE;
        }
    }

    fread(&temp.size_of_data, sizeof(temp.size_of_data), 1,fp);
    if (temp.size_of_data <= 0) {
        fprintf(stderr, "[ERROR] size of data is wrong .\n");
        return EXIT_FAILURE;
    }

    *chunk = temp;
    return EXIT_SUCCESS;
}
