/*
 *  wfh.h
 */

#ifndef WFH_
#define WFH_

typedef enum {
    kIsChar   = 80,
    kIsShort  = 160,
    kIsInt    = 320,
    kIsFloat  = 321,
    kIsDouble = 640,
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
    RiffChunk r;
    FmtChunk  f;
    DataChunk d;
} WavFileHeader;
#pragma pack(pop)

typedef struct WavData {
    WavFileHeader hdr;
    DataTypes     data_type;
    unsigned int  num_of_samples;
    unsigned int  bytes_per_sample;
    double        buf_abs_limit;
    double        *datas[];
} WavData;


// prottype
FILE *open_wav(char *filename, OpenFlags flag);
int close_wav(FILE *fp);
int read_riff_chunk(FILE *fp, RiffChunk *chunk);
int read_fmt_chunk(FILE *fp, FmtChunk *chunk);
int read_data_chunk(FILE *fp, DataChunk *chunk);
int read_wav_file_header(FILE *fp, WavFileHeader *header);
WavData *new_wav_data(FILE *fp);
void destroy_wav_data(WavData *wav);


#endif   // WFH_
