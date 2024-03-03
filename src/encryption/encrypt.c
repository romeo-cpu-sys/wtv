#include "../includes/encrypt.h"
#include "../includes/keygen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#define KEY_SIZE 256

void handleErrors(void);

void encrypt(const char *input_file, const char *output_file) {
    unsigned char key[KEY_SIZE / 8]; // declare key var
    generate_key(key);

    FILE *in_file = fopen(input_file, "rb");
    FILE *out_file = fopen(output_file, "wb");
    if (in_file == NULL || out_file == NULL) {
        fprintf(stderr, "Error opening files\n");
        exit(1);
    }

    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, KEY_SIZE, &aes_key) < 0) {
        fprintf(stderr, "AES_set_encrypt_key error\n");
        exit(1);
    }

    unsigned char in_data[AES_BLOCK_SIZE];
    unsigned char out_data[AES_BLOCK_SIZE];

    while (1) {
        int bytes_read = fread(in_data, 1, AES_BLOCK_SIZE, in_file);
        if (bytes_read <= 0) break;

        if (bytes_read < AES_BLOCK_SIZE) {
            memset(in_data + bytes_read, 0, AES_BLOCK_SIZE - bytes_read);
        }

        AES_cbc_encrypt(in_data, out_data, AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);
        fwrite(out_data, 1, AES_BLOCK_SIZE, out_file);
    }

    fclose(in_file);
    fclose(out_file);
}

void handleErrors(void) {
    fprintf(stderr, "Error occurred\n");
    exit(1);
}
