#include "../includes/keygen.h"
#include "../includes/encrypt.h"
#include "../includes/decrypt.h"
#include "../includes/fileio.h"
#include <stdio.h>

int main() {
    //gen a random key and write it to a file
    unsigned char key[KEY_SIZE / 8];
    generateKey(key);
    writeKeyToFile("encryption_key.bin", key);

    encrypt("plaintext.txt", "encrypted.bin", key, "encryption_key.bin");

    decrypt("encrypted.bin", "decrypted.txt", key, "encryption_key.bin");

    size_t decrypted_size;
    unsigned char *decrypted_data = readFile("decrypted.txt", &decrypted_size);
    printf("decrypted content:\n%s\n", decrypted_data);
    free(decrypted_data);

    return 0;
}
