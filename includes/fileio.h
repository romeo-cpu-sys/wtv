#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>

void writeFile(const char *filename, const unsigned char *data, size_t size);
unsigned char *readFile(const char *filename, size_t *size);

#endif 
