#pragma once

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TRY_ALLOC(ptr, type, count)                                  \
    type *ptr = (type *)malloc(sizeof(type) * count);                \
    if (ptr == NULL)                                                 \
    {                                                                \
        fprintf(stderr, "Failed to allocate memory for %s\n", #ptr); \
        return ptr;                                                  \
    }

#define TRY_SDL(expr)                                        \
    if (!(expr))                                             \
    {                                                        \
        printf("SDL Error: %s %s\n", #expr, SDL_GetError()); \
        return false;                                        \
    }

#define STRUCT_FIELD_SIZE(type, member) sizeof(((type){0}).member) / 4

#define STRUCT_FIELD_OFFSET(type, member) (void *)offsetof(type, member)

char *File_Open(const char *path);