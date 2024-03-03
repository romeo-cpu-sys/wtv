#include "../includes/fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

void handleFileError(const char *message, FILE *file) {
    if (file != NULL) {
        fclose(file);
    }
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

void writeFile(const char *filename, const unsigned char *data, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        handleFileError("unable to open file for writing", NULL);
    }
    
    size_t bytes_written = fwrite(data, 1, size, file);
    if (bytes_written != size) {
        handleFileError("unable to write to file", file);
    }
    
    fclose(file);
}

unsigned char *readFile(const char *filename, size_t *size) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        handleFileError("unable to open file for reading", NULL);
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    unsigned char *buffer = (unsigned char *)malloc(file_size);
    if (buffer == NULL) {
        handleFileError("memory allocation failed", file);
    }
    
    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        free(buffer);
        handleFileError("unable to read from file", file);
    }
    
    fclose(file);
    *size = bytes_read;
    return buffer;
}
