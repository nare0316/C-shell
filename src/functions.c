#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "../headers/functions.h"
#include "../headers/my_lib.h"


bool is_custom_command(const char *command) {
    if (!strcmp(command, "cd") || !strcmp(command, "help") || !strcmp(command, "history") ||
        !strcmp(command, "pwd") || !strcmp(command, "echo") ||
        !strcmp(command, "clear") || !strcmp(command, "my_setenv") ||
        !strcmp(command, "my_unsetenv") || !strcmp(command, "chprompt") || !strcmp(command, "my_getenv") || !strcmp(command, "printenv")) {return true;}

    return false;
}

void print_prompt() {
    int i = 0;
    printf(SET_GREEN);
    while (prompt[i] != ':') {
        putchar(prompt[i]);
        ++i;
    }
    printf(UNSET_GREEN);
    putchar(prompt[i]);
    ++i;
    printf(SET_BLUE);
    while (prompt[i] != '$') {
        putchar(prompt[i]);
        ++i;
    }
    printf(UNSET_BLUE);
    putchar(prompt[i]);
    putchar(' ');
}

void chprompt(const char *prom) {
    char *new_prompt = (char*)calloc(PROMP_LEN, sizeof(char));
    if (new_prompt == NULL) {
        perror("Not enough memory.");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    while (*prom != '\0') {
        new_prompt[i] = *prom;
        ++i;
        ++prom;
    }
    char *contin = strchr(prompt, ':');
    strcat(new_prompt, contin);
    memset(prompt, 0, strlen(prompt));
    strcpy(prompt, new_prompt);
    free(new_prompt);
}
char *my_getenv(const char*name) {
    int bytesR = 0;
    int t = fseek(streamEnv, 0, SEEK_END);
    if (t == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }
    long size = ftell(streamEnv);
    if (size == -1) {
        perror("ftell failed");
        exit(EXIT_FAILURE);
    }
    int number = size/sizeof(Env);

    t = fseek(streamEnv, 0, SEEK_SET);
    if (t == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }

    Env env;
    for (int i = 0; i < number; ++i) {
        memset(&env, 0, sizeof(Env));
        bytesR = fread(&env, 1, sizeof(Env), streamEnv);
        if (bytesR == -1) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }
        if (strcmp(env.name, name) == 0) {
            char *get_value = calloc(100, sizeof(char));
            if (get_value == NULL) {
                perror("Not enough memory!");
                exit(EXIT_FAILURE);
            }
            strcat(get_value, env.value);
            return get_value;
        }
    }
    printf("That name \"%s\" was not found in the environment.\n", name);
}

char *pwd() {
    return my_getenv("PWD");
} 

void my_setenv(const char *name, const char *value) {
    int bytesR = 0;
    size_t bytesW = 0;
    int t = fseek(streamEnv, 0, SEEK_END);
    if (t == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }
    long size = ftell(streamEnv);
    if (size == -1) {
        perror("ftell failed");
        exit(EXIT_FAILURE);
    }
    int number = size/sizeof(Env);

    t = fseek(streamEnv, 0, SEEK_SET);
    if (t == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }

    Env env;
    for (int i = 0; i < number; ++i) {
        memset(&env, 0, sizeof(Env));
        bytesR = fread(&env, 1, sizeof(Env), streamEnv);
        if (bytesR == -1) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }
        if (strcmp(env.name, name) == 0) {
            memset(env.value, 0, strlen(env.value));
            strcpy(env.value, value);
            t = fseek(streamEnv, -sizeof(Env), SEEK_CUR);
            if (t == -1) {
                perror("fseek failed");
                exit(EXIT_FAILURE);
            }
            bytesW = fwrite(&env, 1, sizeof(Env), streamEnv);
            if (bytesW == -1) {
                perror("fwrite failed");
                exit(EXIT_FAILURE);
            }
            return;
        }
    }
    memset(&env, 0, sizeof(Env));
    strcpy(env.name, name);
    strcpy(env.value, value);
    bytesW = fwrite(&env, 1, sizeof(Env), streamEnv);
    if (bytesW == -1) {
        perror("fwrite failed");
        exit(EXIT_FAILURE);
    }
    
}



void cd(char *path) {
    if (strcmp(path, ".") == 0) {
        return;
    }
    if (!strcmp(working_path, "/") && !strcmp(path, "..") ) {
        return;
    }
    if (flag) {
        int cls = close(fd);
        if (cls == -1) {
            perror("close failed");
        }
        flag = 0;
    }
    memset(working_path, 0, strlen(working_path));
    strcat(working_path, my_getenv("PWD"));
    if (strcmp(path, "..") == 0) {
        int i = strlen(working_path);
        while(working_path[i] != '/') {
            working_path[i] = 0;
            --i;
        }
        working_path[i] = 0;
        fd = open(path, __O_DIRECTORY);
        if (fd == -1) {
            printf("%s.\n", strerror(errno));
            return;
        }
        flag = 1;
    } else if (path[0] == '/') {
        fd = open(path, __O_DIRECTORY);
        if (fd == -1) {
            printf("%s.\n", strerror(errno));
            return;
        }
        flag = 1;
        memset(working_path, 0, strlen(working_path));
        strcat(working_path, path);
    } else {
        if (path[0] == '.' && path[1] == '/') {
            path += 2;
        }
        if  (working_path[strlen(working_path)-1] != '/') {
            working_path[strlen(working_path)] = '/';
        }
        strcat(working_path, path);
        fd = open(working_path, __O_DIRECTORY);
        if (fd == -1) {
            printf("%s.\n", strerror(errno));
            return;
        }
        flag = 1;
    }
    //update pwd
    my_setenv("PWD", working_path);

    //update prompt
    char *ptr1 = NULL;
    char *ptr2 = strchr(prompt, (int)':');
    if (ptr2 == NULL) {
        perror("strchr failed.");
        exit(EXIT_FAILURE);
    }
    ptr2 += 1;
    memset(ptr2, 0, strlen(working_path));
    if ((ptr1 = strstr(working_path, HOME_PATH)) == NULL) {
        strcat(ptr2, working_path);
        prompt[strlen(prompt)] = '$';
    } else {
        int j = strlen(HOME_PATH);
        int i = strlen(prompt);
        prompt[i] = '~';
        for (i += 1 ; working_path[j] != '\0'; ++i, ++j) {
            prompt[i] = working_path[j];
        }
        prompt[i] = '$';
    }

} 

void help(const char* command) {
    char another_command[100] = "help ";
    if (!strcmp(command, "help")) {
        printf("help: Show information about available commands.\n");
        printf("Usage: help [command]\n");
    } else if (!strcmp(command, "cd")) {
        printf("cd: Change the current working directory.\n");
        printf("Usage: cd <directory>\n");
    } else if (!strcmp(command, "pwd")) {
        printf("pwd: Print the current working directory.\n");
        printf("Usage: pwd\n");
    } else if (!strcmp(command, "history")) {
        printf("history: Display the command history.\n");
        printf("Usage: history\n");
    }  else if (!strcmp(command, "clear")) {
        printf("clear: Clear the terminal screen.\n");
        printf("Usage: clear\n");
    }  else if (!strcmp(command, "my_setenv")) {
        printf("setenv: Set an environment variable.\n");
        printf("Usage: setenv <variable> <value>\n");
    } else if (!strcmp(command, "my_unsetenv")) {
        printf("unsetenv: Remove an environment variable.\n");
        printf("Usage: unsetenv <variable>\n");
    } else if (!strcmp(command, "chprompt")) {
        printf("chprompt: Change the shell prompt.\n");
        printf("Usage: chprompt <new_prompt>\n");
    } else if (!strcmp(command, "echo")) {
        printf("echo: Display a message or value.\n");
        printf("Usage: echo <message>\n");
    } else if (!strcmp(command, "my_getenv")) {
        printf("getenv: Returns the value of name in the environment.\n");
        printf("Usage: my_getenv <name>\n");
    } else if (!strcmp(command, "printenv")) {
        printf("printenv: Print all environment.\n");
        printf("Usage: printenv\n");
    } else {
        strcat(another_command,command);
        system(another_command);
    }
}



void printenv() {
    size_t bytesR = 0;
    int t = fseek(streamEnv, 0, SEEK_END);
    if (t == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }
    long size = ftell(streamEnv);
    if (size == -1) {
        perror("ftell failed");
        exit(EXIT_FAILURE);
    }
    int number = size/sizeof(Env);

    t = fseek(streamEnv, 0, SEEK_SET);
    if (t == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }

    Env env;
    for (int i = 0; i < number; ++i) {
        memset(&env, 0, sizeof(Env));
        bytesR = fread(&env, 1, sizeof(Env), streamEnv);
        if (bytesR == -1) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }
        printf("%s=%s\n", env.name, env.value);
    }
}

void history(FILE *stream) {
    int fs = fseek(stream, 0, SEEK_END);
    if (fs == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }
    long sizeFile = ftell(stream);
    if (sizeFile == -1) {
        perror("ftell failed");
        exit(EXIT_FAILURE);
    }
    char *buffer = calloc(sizeFile, sizeof(char));
    if (buffer == NULL) {
        perror("not enough memory!");
        exit(EXIT_FAILURE);
    }
    fs = fseek(stream, 0, SEEK_SET);
    if (fs == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }
    size_t bytes = fread(buffer, 1, sizeFile, stream);
    if (bytes == -1) {
        perror("fwrite failed");
        exit(EXIT_FAILURE);
    }
    printf("%s", buffer);
    free(buffer);
}


void my_unsetenv(const char *name) {
    int bytesR = 0;
    size_t bytesW = 0;
    int t = fseek(streamEnv, 0, SEEK_END);
    if (t == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
        
    }
    long size = ftell(streamEnv);
    if (size == -1) {
        perror("ftell failed");
        exit(EXIT_FAILURE);
    }
    int number = size/sizeof(Env);

    Env *envArr = (Env*)calloc(number, sizeof(Env));
    if (envArr == NULL) {
        perror("Not enough memory!");
        exit(EXIT_FAILURE);
    }

    t = fseek(streamEnv, 0, SEEK_SET);
    if (t == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < number; ++i) {
        bytesR = fread(&envArr[i], 1, sizeof(Env), streamEnv);
        if (bytesR == -1) {
            perror("fread failed");
        }
    }
    t = truncate(FILE_ENV, 0);
    if (t == -1) {
        perror("truncate failed");
    }

    t = fseek(streamEnv, 0, SEEK_SET);
    if (t == -1) {
        perror("fseek failed");
        exit(EXIT_FAILURE);
    }

    int flag = 1;
    for (int i = 0; i < number; ++i) {
        if (strcmp(envArr[i].name, name) == 0) {
            flag = 0;
            continue;
        }
        bytesW = fwrite(&envArr[i], 1, sizeof(Env), streamEnv);
        if (bytesW == -1) {
            perror("fread failed");
        }
    }
    fflush(streamEnv);
    if (flag) {
        printf("That name \"%s\" was not found in the environment.\n", name);
    }
}

void echo(char **args, int size) {
    if (size == 2 && args[1][0] == '$') {
        args[1] = &args[1][1];
        char *echo_value = my_getenv(args[1]);
        printf("%s\n", echo_value);
        free(echo_value);
        return;
    }
    for (int i = 1; i < size; ++i) {
        printf("%s ", args[i]);
    }
    printf("\n");
}

void clear() {
    printf("\033[H\033[J");
    fflush(stdout); 
}



void parser(char *command, char *args[100]) {
    char *token = NULL;
    char *sym = NULL;
    if (strncmp(command, "echo ", 5) == 0 && (sym = strchr(command, (int)'\"')) != NULL && (sym = strchr(sym, (int)'\"'))) {
        token = strtok(command, "\"");
        args[0] = token;
        args[1] = strtok(NULL, "\"");
        echo(args, 2);
        return;
    }

    token = strtok(command, " \n\t");
    int i = 0;
    while (token != NULL) {
        args[i] = token;
        token = strtok(NULL, " \n\t");
        ++i;
    }
    args[i] = NULL;
    if (is_custom_command(args[0])) { 
        if (i == 2 && strcmp(args[0], "chprompt") == 0) {
            chprompt(args[1]);
        } else if (i == 2 && strcmp(args[0], "cd") == 0 && strcmp(args[1], "~")) {
            cd(args[1]);
        } else if ( (i == 1 && !strcmp(args[0], "cd") ) || ( i == 2 && !strcmp(args[0], "cd") && !strcmp(args[1], "~") )) {
            cd(HOME_PATH);
        } else if (i == 1 && strcmp(args[0], "pwd") == 0) {
            printf("%s\n", pwd());
        } else if (i == 2 && strcmp(args[0], "help") == 0) {
            help(args[1]);
        } else if (i == 1 && strcmp(args[0], "history") == 0) {
            history(streamHistory);
        } else if (i == 3 && strcmp(args[0], "my_setenv") == 0) {
            my_setenv(args[1], args[2]);
        } else if (i == 2 && strcmp(args[0], "my_getenv") == 0) {
            char *get_value =  my_getenv(args[1]);
            printf("%s\n",get_value);
            free(get_value);
        }  else if (i == 2 && strcmp(args[0], "my_unsetenv") == 0) {
            my_unsetenv(args[1]);
        }  else if (strcmp(args[0], "echo") == 0) {
            echo(args, i);
        }  else if (strcmp(args[0], "clear") == 0) {
            clear();
        }  else if (i == 1 && strcmp(args[0], "printenv") == 0) {
            printenv();
        }

    } else {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            char command_program[100] = {0};
            strcpy(command_program, "/bin/");
            strcat(command_program, args[0]);
            if (execvp(command_program, args) == -1) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } else {
            int status;
            waitpid(pid, &status, 0);  
        }
    }
}

