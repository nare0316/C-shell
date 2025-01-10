#ifndef MY_DEFINES_H  
#define MY_DEFINES_H

#include <stdbool.h>

int fd = 0;
int flag = 0;
extern FILE *streamHistory;
extern FILE *streamEnv;
typedef struct env {
    char name[100];
    char value[100];
} Env;

bool is_custom_command(const char *);
void print_prompt();
void chprompt(const char *);
char *my_getenv(const char *);
char *pwd();
void my_setenv(const char *, const char *);
void cd(char *);
void help(const char *);
void printenv();
void history(FILE *);
void my_unsetenv(const char *);
void echo(char **, int);
void clear();
void parser(char *, char *args[100]);

#endif  // End of MY_DEFINES_H