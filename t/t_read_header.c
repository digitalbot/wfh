#include <stdio.h>
#include <string.h>

#include "../wav_file_handle.h"


int main(int argc, char *argv[]) {
    FILE *fp;
    char *filename = "gt1.wav";
    OpenFlags flag = kIsRead;
    RiffChunk *riff_chunk;
    WavFileHeader *wav_file_header;

    // open test
    if ((fp = open_wav(filename, flag)) != NULL) {
        printf("ok -- open\n");
    }
    else {
        printf("fail -- open\n");
    }

    // read riff chunk
    if (read_riff_chunk(fp, riff_chunk) == 0) {
        printf("ok -- read_riff\n");
    }
    else {
        printf("fail -- read_riff\n");
    }

    // close test
    if (close_wav(fp) == 0) {
        printf("ok -- close\n");
    }
    else {
        printf("fail -- close\n");
    }
}
