#include "../includes/keygen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define KEY_SIZE 256

void handleKeygenError(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

void generateKey(unsigned char *key) {
    srand(time(NULL));
    for (int i = 0; i < KEY_SIZE / 8; i++) {
        key[i] = rand() % 256;
    }
}

void writeKeyToFile(const char *filename, const unsigned char *key) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        handleKeygenError("unable to open file for writing");
    }

    size_t bytes_written = fwrite(key, 1, KEY_SIZE / 8, file);
    if (bytes_written != KEY_SIZE / 8) {
        fclose(file);
        handleKeygenError("unable to write key > file");
    }

    fclose(file);
}

/*
00000000  4f 63 65 61 6e 53 70 65  65 63 68 4d 65 2d 3e 43  |OceanSpeechMe->C|
00000010  48 3b 5e 40 7d 32 38 2c  26 2d 25 3e 2f 3c 21 23  |H;^@}28,&-%>/<!#|
00000020
*/
