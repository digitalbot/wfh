#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../wav_file_handle.h"

void t_read_riff_chunk(FILE *fp);
void t_read_fmt_chunk(FILE *fp);
void t_read_data_chunk(FILE *fp);

int main(int argc, char *argv[]) {
    FILE *fp;
    char *filename = "gt1.wav";
    OpenFlags flag = kIsRead;
    FmtChunk *fmt_chunk;
    //WavFileHeader *wav_file_header;

    if ((fp = open_wav(filename, flag)) == NULL) {
        return EXIT_FAILURE;
    }

    // read riff chunk
    t_read_riff_chunk(fp);

    // read fmt chunk
    t_read_fmt_chunk(fp);

    // read data chunk
    t_read_data_chunk(fp);

    if (close_wav(fp)) {
        return EXIT_FAILURE;
    };

    return EXIT_SUCCESS;
}

void t_read_riff_chunk(FILE *fp) {
    RiffChunk riff_chunk;
    assert(read_riff_chunk(fp, &riff_chunk) == EXIT_SUCCESS);
    assert(memcmp(riff_chunk.file_type, "RIFF", 4) == 0);
    assert(riff_chunk.size_of_riff == 1574176);
    assert(memcmp(riff_chunk.riff_type, "WAVE", 4) == 0);
    return;
}

void t_read_fmt_chunk(FILE *fp) {
    FmtChunk fmt_chunk;
    assert(read_fmt_chunk(fp, &fmt_chunk) == EXIT_SUCCESS);
    assert(memcmp(fmt_chunk.chunk_type, "fmt ", 4) == 0);
    assert(fmt_chunk.size_of_fmt == 16);
    assert(fmt_chunk.format_tag == 1);
    assert(fmt_chunk.num_of_channels == 1);
    assert(fmt_chunk.samples_per_sec == 44100);
    assert(fmt_chunk.bits_per_sample == 16);
    assert(fmt_chunk.size_of_block = 2);
    assert(fmt_chunk.bytes_per_sec == 88200);
    return;
}

void t_read_data_chunk(FILE *fp) {
    DataChunk data_chunk;
    assert(read_data_chunk(fp, &data_chunk) == EXIT_SUCCESS);
    assert(memcmp(data_chunk.chunk_type, "data", 4) == 0);
    assert(data_chunk.size_of_data == 1574140);
    return;
}
