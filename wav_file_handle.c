/*
 * wav_file_handle.c
 */


#include <stdio.h>
#include <string.h>

#include "wav_file_handle.h"


int OpenWav(FILE *fp, char *filename, OpenFlags flag) {
    switch (flag) {
    case 1:   //read
        if ((fp = fopen(filename, "rb")) == NULL) {
            fprintf(stderr, "[ERROR] Can't open %s.\n", filename);
            return -1;
        }
        break;
    case 2:   //write
        if ((fp = fopen(filename, "wb")) == NULL) {
            fprintf(stderr, "[ERROR] Can't open %s.\n", filename);
            return -1;
        }
        break;
    case 3:   //read & write
        if ((fp = fopen(filename, "rwb")) == NULL) {
            fprintf(stderr, "[ERROR] Can't open %s.\n", filename);
            return -1;
        }
        break;
    }
    return 0;
}

int CloseWav(FILE *fp) {
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] something is wrong!\n");
        return -1;
    }
    fclose(fp);
    return 0;
}
