#include "../includes/decrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/md5.h>

#define BUFFER_SIZE 4096
#define KEY_SIZE 256

void handleErrors(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

void verifyChecksum(const char *decrypted_file, const char *checksum_file) {
    unsigned char decrypted_checksum[MD5_DIGEST_LENGTH];
    MD5_CTX mdContext;
    MD5_Init(&mdContext);
    FILE *decrypted_fp = fopen(decrypted_file, "rb");
    if (decrypted_fp == NULL) {
        handleErrors("unable to open decrypted file for checksum verification");
    }
    unsigned char data[1024];
    int bytes;
    while ((bytes = fread(data, 1, sizeof(data), decrypted_fp)) != 0) {
        MD5_Update(&mdContext, data, bytes);
    }
    MD5_Final(decrypted_checksum, &mdContext);
    fclose(decrypted_fp);

    unsigned char stored_checksum[MD5_DIGEST_LENGTH];
    FILE *checksum_fp = fopen(checksum_file, "rb");
    if (checksum_fp == NULL) {
        handleErrors("unable to open checksum file");
    }
    fread(stored_checksum, 1, MD5_DIGEST_LENGTH, checksum_fp);
    fclose(checksum_fp);

    if (memcmp(decrypted_checksum, stored_checksum, MD5_DIGEST_LENGTH) != 0) {
        fprintf(stderr, "Checksum verification unable: Decrypted data may be corrupted or key is incorrect\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Checksum verification successful: Decrypted data integrity preserved\n");
    }
}

void decrypt(const char *input_file, const char *output_file, const unsigned char *key, const char *checksum_file) {
    FILE *in_file = fopen(input_file, "rb");
    if (in_file == NULL) {
        handleErrors("unable to open input file");
    }

    FILE *out_file = fopen(output_file, "wb");
    if (out_file == NULL) {
        fclose(in_file);
        handleErrors("unable to open output file");
    }

    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    AES_KEY aes_key;
    if (AES_set_decrypt_key(key, KEY_SIZE, &aes_key) < 0) {
        fclose(in_file);
        fclose(out_file);
        handleErrors("unable to set AES decryption key");
    }

    unsigned char in_data[BUFFER_SIZE];
    unsigned char out_data[BUFFER_SIZE];
    int bytes_read, decrypted_length;

    while ((bytes_read = fread(in_data, 1, BUFFER_SIZE, in_file)) > 0) {
        AES_cbc_encrypt(in_data, out_data, bytes_read, &aes_key, iv, AES_DECRYPT);
        decrypted_length = bytes_read;
        if (fwrite(out_data, 1, decrypted_length, out_file) != decrypted_length) {
            fclose(in_file);
            fclose(out_file);
            handleErrors("unable to write decrypted data to output file");
        }
    }

    if (bytes_read < 0) {
        fclose(in_file);
        fclose(out_file);
        handleErrors("Error reading input file");
    }

    fclose(in_file);
    fclose(out_file);

    verifyChecksum(output_file, checksum_file);
}

