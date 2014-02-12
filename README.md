wfh
========

wfh is Wav File Handler!  

This library lets you control wav file more easy.

This is a c library.


intro
-----

Put wfh.c and wfh.h in your project! only!


usage
-----

### initialize with file pointer
        WavData *wav = new_wav_data(fp);

### destroy

You have to destroy WavData objects!
        destroy_wav_data(wav);


That is all.
