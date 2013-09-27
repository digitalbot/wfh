/*
 *  wav_file_handle.h
 */

#ifndef WAV_FILE_HANDLE_
#define WAV_FILE_HANDLE_

#define MONORAL 1
#define STEREO  2

typedef enum {
    kIsChar   = 8,
    kIsShort  = 16,
    kIsFloat  = 32,
    kIsDouble = 64,
} DataTypes;

typedef enum {
    kIsRead = 1,
    kIsWrite = 2,
} OpenFlags;

#pragma pack(push, 1)
typedef struct RiffChunk {
    char         file_type[4];
    unsigned int size_of_riff;
    char         riff_type[4];
} RiffChunk;

typedef struct FmtChunk {
    char           chunk_type[4];
    unsigned int   size_of_fmt;
    unsigned short format_tag;
    unsigned short num_of_channels;
    unsigned int   samples_per_sec;
    unsigned int   bytes_per_sec;
    unsigned short size_of_block;
    unsigned short bits_per_sample;
} FmtChunk;

typedef struct DataChunk {
    char         chunk_type[4];
    unsigned int size_of_data;
} DataChunk;

typedef struct WavFileHeader {
    RiffChunk riff_chunk;
    FmtChunk  fmt_chunk;
    DataChunk data_chunk;
} WavFileHeader;
#pragma pack(pop)

typedef struct WavData {
    unsigned short num_of_channels;
    unsigned int   num_of_samples;
    unsigned int   samples_per_sec;
    unsigned int   bytes_per_sec;
    unsigned short size_of_block;
    unsigned int   bytes_per_sample;
    double         buf_abs_limit;
    double         *data;
    double         *data_r;
} WavData;


// prottype
FILE *open_wav(char *filename, OpenFlags flag);
int close_wav(FILE *fp);
int read_riff_chunk(FILE *fp, RiffChunk *chunk);
int read_fmt_chunk(FILE *fp, FmtChunk *chunk);
int read_data_chunk(FILE *fp, DataChunk *chunk);
int read_wav_file_header(FILE *fp, WavFileHeader *header);


#endif   // WAV_FILE_HANDLE_
