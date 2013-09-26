CC					= clang
CFLAGS				= -O4 -I/usr/local/include
DEST				= /usr/local/bin
LDFLAGS				= -L/usr/local/lib

TARGETS				= t_read_header t_open_wav

all:				$(TARGETS)
.SUFFIXES:	 		.c .o

t_read_header:		t_read_header.o wav_file_handle.o
					$(CC) -o $@ $^

t_open_wav:			t_open_wav.o wav_file_handle.o
					$(CC) -o $@ $^

.c.o:
					$(CC) $(CFLAGS) -c $<

wav_file_handle.o:	../wav_file_handle.c
					$(CC) $(CFLAGS) -c $<


.PHONY:				clean
clean:
					$(RM) *.o *- $(TARGETS)
