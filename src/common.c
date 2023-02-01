#include "common.h"

char *File_Open(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file: %s\n", path);
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    TRY_ALLOC(buffer, char, length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}