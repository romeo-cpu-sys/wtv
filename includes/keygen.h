#ifndef KEYGEN_H
#define KEYGEN_H

#define KEY_SIZE 256

void generateKey(unsigned char *key);
void writeKeyToFile(const char *filename, const unsigned char *key);

#endif
