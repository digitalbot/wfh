#include <stdio.h>
#include <string.h>

#include "../wav_file_handle.h"


int main(int argc, char *argv[]) {
    FILE *fp;
    char *filename = "gt1.wav";
    OpenFlags flag = kIsRead;
    WavFileHeader *wav_file_header;

    // open test
    if (OpenWav(fp, filename, flag) == 0) {
        printf("ok -- open\n");
    }
    else {
        printf("fail -- open\n");
    }

    if (ReadHeader(fp, wav_file_header) == 0) {
        printf("ok -- read\n");
    }
    else {
        printf("fail -- read\n");
    }

    // close test
    if (CloseWav(fp) == 0) {
        printf("ok -- close\n");
    }
    else {
        printf("fail -- close\n");
    }
}

