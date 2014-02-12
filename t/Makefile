CC				= clang
CFLAGS			= -O4 -I/usr/local/include -Wall
DEST			= /usr/local/bin
LDFLAGS			= -L/usr/local/lib

TARGETS			= t_read_header t_open_wav t_new_wav_data

all:				$(TARGETS)
.SUFFIXES:	 		.c .o

t_new_wav_data:		t_new_wav_data.o wfh.o
					$(CC) -o $@ $^

t_read_header:		t_read_header.o wfh.o
					$(CC) -o $@ $^

t_open_wav:			t_open_wav.o wfh.o
					$(CC) -o $@ $^

.c.o:
					$(CC) $(CFLAGS) -c $<

wfh.o:				../wfh.c
					$(CC) $(CFLAGS) -c $<

.PHONY:				test
test:
					make all && for f in $(TARGETS); do ./$$f; done;

.PHONY:				clean
clean:
					$(RM) *.o *- $(TARGETS)
