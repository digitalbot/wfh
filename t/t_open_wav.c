#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../wfh.h"


int main(int argc, char *argv[]) {
    FILE *fp;
    char *readfile  = "gt1.wav";
    char *writefile = "test.wav";

    // read
    assert((fp = open_wav(readfile, kIsRead)) != NULL);

    // close
    assert(! close_wav(fp));

    // write
    assert((fp = open_wav(writefile, kIsWrite)) != NULL);

    // close
    assert(! close_wav(fp));
    assert(! remove(writefile));

    return EXIT_SUCCESS;
}
