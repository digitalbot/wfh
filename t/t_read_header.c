#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../wfh.h"

void t_read_riff_chunk(FILE *fp);
void t_read_fmt_chunk(FILE *fp);
void t_read_data_chunk(FILE *fp);
void t_read_wav_file_header(FILE *fp);

int main(int argc, char *argv[]) {
    FILE *fp;
    char *filename = "gt1.wav";
    OpenFlags flag = kIsRead;

    if ((fp = open_wav(filename, flag)) == NULL) {
        return EXIT_FAILURE;
    }

    t_read_riff_chunk(fp);
    t_read_fmt_chunk(fp);
    t_read_data_chunk(fp);
    t_read_wav_file_header(fp);

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

void t_read_wav_file_header(FILE *fp) {
    WavFileHeader header;
    assert(read_wav_file_header(fp, &header) == EXIT_SUCCESS);
    assert(memcmp(header.r.file_type, "RIFF", 4) == 0);
    assert(header.r.size_of_riff == 1574176);
    assert(memcmp(header.r.riff_type, "WAVE", 4) == 0);
    assert(memcmp(header.f.chunk_type, "fmt ", 4) == 0);
    assert(header.f.size_of_fmt == 16);
    assert(header.f.format_tag == 1);
    assert(header.f.num_of_channels == 1);
    assert(header.f.samples_per_sec == 44100);
    assert(header.f.bits_per_sample == 16);
    assert(header.f.size_of_block = 2);
    assert(header.f.bytes_per_sec == 88200);
    assert(memcmp(header.d.chunk_type, "data", 4) == 0);
    assert(header.d.size_of_data == 1574140);
    return;
}
