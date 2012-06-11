#include <stdio.h>
#include <string.h>

#include "../wav_file_handle.h"


int main(int argc, char *argv[]) {
    FILE *fp;
    char *filename = "gt1.wav";
    OpenFlags flag = kIsRead;

    // open test
    if (OpenWav(filename, fp, flag) == 0) {
        printf("ok -- open\n");
    }
    else {
        printf("fail -- open\n");
    }

    // close test
    if (CloseWav(fp) == 0) {
        printf("ok -- close\n");
    }
    else {
        printf("fail -- close\n");
    }
}

