#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../wav_file_handle.h"


int main(int argc, char *argv[]) {
    FILE *fp;
    char *filename = "gt1.wav";
    OpenFlags flag = kIsRead;
    RiffChunk *riff_chunk;
    //FmtChunk *fmt_chunk;
    //WavFileHeader *wav_file_header;

    assert((fp = open_wav(filename, flag)) != NULL);

    // read riff chunk
    assert(! read_riff_chunk(fp, riff_chunk));

    // read fmt chunk
    //assert(! read_fmt_chunk(fp, fmt_chunk));

    assert(! close_wav(fp));

    return EXIT_SUCCESS;
}
