/*
 *  wfh.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "wfh.h"


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
    if (temp.format_tag != 3 && temp.format_tag != 1) {
        fprintf(stderr, "[ERROR] format tag <%d> is wrong.\n", temp.format_tag);
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
    unsigned char seeked_flg = 0;
    while (1) {
        fread(&temp.chunk_type, sizeof(temp.chunk_type), 1, fp);
        if (memcmp(temp.chunk_type, "data", 4)) {
            if (memcmp(temp.chunk_type, "fact", 4) == 0) {
                fseek(fp, 12, SEEK_CUR);
            }
            else if (! seeked_flg) {
                fseek(fp, - sizeof(temp.chunk_type), SEEK_CUR);

                // extended fmt chunk
                unsigned short size_of_ext = 0;
                fread(&size_of_ext, sizeof(size_of_ext), 1, fp);
                fseek(fp, size_of_ext, SEEK_CUR);
            }
            else {
                fprintf(stderr, "[ERROR] This file has not 'data' chunk.\n");
                return EXIT_FAILURE;
            }
            seeked_flg++;
        }
        else {
            break;
        }
    }

    fread(&temp.size_of_data, sizeof(temp.size_of_data), 1,fp);
    if (temp.size_of_data <= 0) {
        fprintf(stderr, "[ERROR] size of data is wrong.\n");
        return EXIT_FAILURE;
    }

    *chunk = temp;
    return EXIT_SUCCESS;
}

int read_wav_file_header(FILE *fp, WavFileHeader *header) {
    RiffChunk riff_chunk;
    FmtChunk  fmt_chunk;
    DataChunk data_chunk;

    if (read_riff_chunk(fp, &riff_chunk)) {
        return EXIT_FAILURE;
    }
    if (read_fmt_chunk(fp, &fmt_chunk)) {
        return EXIT_FAILURE;
    }
    if (read_data_chunk(fp, &data_chunk)) {
        return EXIT_FAILURE;
    }

    header->r = riff_chunk;
    header->f = fmt_chunk;
    header->d = data_chunk;
    return EXIT_SUCCESS;
}

static int _seek_to_beginning_of_data(FILE *fp) {
    DataChunk temp;
    if (read_data_chunk(fp, &temp)) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

WavData *new_wav_data(FILE *fp) {
    WavFileHeader h;
    if (read_wav_file_header(fp, &h)) {
        return NULL;
    }
    if (_seek_to_beginning_of_data(fp)) {
        return NULL;
    }

    int channels = h.f.num_of_channels;
    int samples  = h.d.size_of_data / h.f.size_of_block;
    int bitrate  = h.f.bits_per_sample;
    WavData *wav = malloc(sizeof(struct WavData) + sizeof(double *) * channels);
    wav->hdr = h;
    wav->data_type = bitrate * 10 + (h.f.format_tag == 3 ? 1 : 0);
    wav->num_of_samples = samples;
    wav->bytes_per_sample = bitrate / CHAR_BIT;

    for (int i = 0; i < channels; i++) {
        wav->datas[i] = malloc(sizeof(double) * wav->num_of_samples);
    }

    void *buf;
    switch (wav->data_type) {
        case kIsDouble:
            wav->buf_abs_limit = 1.0;
            for (int i = 0; i < samples; i++) {
                for (int j = 0; j < channels; j++) {
                    fread(&buf, wav->bytes_per_sample, 1 , fp);
                    wav->datas[j][i] = (double)*(double *)&buf;
                }
            }
            break;

        case kIsFloat:
            wav->buf_abs_limit = 1.0;
            for (int i = 0; i < samples; i++) {
                for (int j = 0; j < channels; j++) {
                    fread(&buf, wav->bytes_per_sample, 1 , fp);
                    wav->datas[j][i] = (double)*(float *)&buf;
                }
            }
            break;

        case kIsInt:
            wav->buf_abs_limit = (1 << bitrate) / 2;
            for (int i = 0; i < samples; i++) {
                for (int j = 0; j < channels; j++) {
                    fread(&buf, wav->bytes_per_sample, 1 , fp);
                    wav->datas[j][i] = (double)*(int *)&buf / wav->buf_abs_limit;
                }
            }
            break;

        case kIsShort:
            wav->buf_abs_limit = (1 << bitrate) / 2;
            for (int i = 0; i < samples; i++) {
                for (int j = 0; j < channels; j++) {
                    fread(&buf, wav->bytes_per_sample, 1 , fp);
                    wav->datas[j][i] = (double)*(short *)&buf / wav->buf_abs_limit;
                }
            }
            break;

        case kIsChar:
            wav->buf_abs_limit = (1 << bitrate) / 2;
            for (int i = 0; i < samples; i++) {
                for (int j = 0; j < channels; j++) {
                    fread(&buf, wav->bytes_per_sample, 1 , fp);
                    wav->datas[j][i] = (double)*(char *)&buf / wav->buf_abs_limit;
                }
            }
            break;

        default:
            fprintf(stderr, "[ERROR] bitrate <%d> is wrong.\n", bitrate);
            break;
    }
    return wav;
}

void destroy_wav_data(WavData *wav) {
    if (wav == NULL) return;
    for (int i = 0; i < wav->hdr.f.num_of_channels; i++) {
        if (wav->datas[i] != NULL) {
            free(wav->datas[i]);
            wav->datas[i] = NULL;
        }
    }
    free(wav);
}
