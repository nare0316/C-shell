#ifndef SHELL2_h
#define SHELL2_h

#include <stdio.h>

#define FILE_ENV "env.txt"
#define FILE_HISTORY "history.txt"
#define HOME_PATH "/home/lenovo"

FILE *streamHistory = NULL;
FILE *streamEnv = NULL;

typedef struct env {
    char name[100];
    char value[100];
} Env;

#endif