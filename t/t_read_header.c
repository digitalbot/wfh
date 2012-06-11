#include <stdio.h>
#include <string.h>

#include "../wav_file_handle.h"


int main(int argc, char *argv[]) {
    FILE *fp;
    char *filename("gt1.wav");
    OpenFlags flag(kIsRead);

    if (OpenWav(filename, fp, flag)) {
        return -1;
    }
    printf("ok\n");
    CloseWav(fp);
}
