#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../wfh.h"

void t_new_wav_data(FILE *fp);

int main(int argc, char *argv[]) {
    FILE *fp;
    char *filename = "gt1.wav";
    OpenFlags flag = kIsRead;

    if ((fp = open_wav(filename, flag)) == NULL) {
        return EXIT_FAILURE;
    }

    t_new_wav_data(fp);

    if (close_wav(fp)) {
        return EXIT_FAILURE;
    };

    return EXIT_SUCCESS;
}


void t_new_wav_data(FILE *fp) {
    WavData *wav = new_wav_data(fp);
    assert(wav);
    destroy_wav_data(wav);
    return;
}
