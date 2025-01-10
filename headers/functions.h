#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define PATH_LEN 200
#define PROMP_LEN 250
#define FILE_ENV "env.txt"
#define FILE_HISTORY "history.txt"
#define HOME_PATH "/home/lenovo"
#define PROMPT  "my_prompt:"
#define SET_GREEN "\033[32m"
#define UNSET_GREEN "\033[0m"
#define SET_BLUE "\033[34m"
#define UNSET_BLUE "\033[0m"

char prompt[PROMP_LEN] = PROMPT;
char working_path[PATH_LEN] = HOME_PATH;

#endif