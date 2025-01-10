#include <stdio.h>
#include <string.h>
#include "./headers/shell.h"

extern char working_path[];
extern void parser(char *command, char *args[100]);
extern void cd(char *path);
extern void print_prompt();

int main() {
    char command[256];
    char *args[100];
    streamHistory = fopen(FILE_HISTORY, "w+");
    if (streamHistory == NULL) {
        perror("fopen failed");
    }
    int bytes = 0;
    int num = 1;

    streamEnv = fopen(FILE_ENV, "w+");
    if (streamEnv == NULL) {
        perror("fopen failed");
    }
    Env env;
    memset(&env, 0, sizeof(Env));
    strcpy(env.name, "PWD");
    strcpy(env.value, working_path);
    size_t byteW = fwrite(&env, 1, sizeof(Env), streamEnv);
    if (byteW == -1) {
        perror("fwrite failed");
    }
    cd(HOME_PATH);

    while (1) { 
        print_prompt();
        memset(command, 0, strlen(command));
        fgets(command, sizeof(command), stdin);
        bytes = fprintf(streamHistory, "%d. %s", num, command);
        if (bytes == -1) {
            perror("write failed");
        }
    
        command[strlen(command)-1] = '\0';
        if (strcmp(command, "exit") == 0)  {
            break;
        }
        parser(command, args);
        ++num;
    }
    fclose(streamHistory);
    fclose(streamEnv);
}