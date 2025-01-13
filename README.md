💻 Custom Shell Project

📌 Features
☑️ Custom Commands: Supports basic commands like cd, pwd, echo, clear, history, help, and more.
☑️ Environment Variable Management: Allows setting (setenv), unsetting (unsetenv), and getting (getenv) environment variables.
☑️ Prompt Customization: Change the shell prompt with the chprompt command.
☑️ History: Keeps track of the command history and displays it using the history command.
☑️ Terminal Management: Includes clear to clean the terminal screen.

📌 Files
The project consists of the following files:

📗 Makefile
This file is used for automating the build process.
Targets:
1. make or make all: Compiles the project and creates the executable customShell.
2. make run: Runs the compiled customShell program.
3. make delFiles: Deletes the env.txt and history.txt files.
4. make del: Deletes the customShell executable.

📗 shell.c
The main entry point of the program, where the shell loop is implemented.
This file initializes the environment, sets up history tracking, and repeatedly prompts the user for commands.
It handles the user input by calling the parser function to interpret the commands and execute them.
The file also contains the logic for reading and writing to the history file and managing the shell prompt.

📗 functions.c
Contains the functions for handling the core shell operations, including custom commands, environment variable management, and terminal operations.
Functions:
1. is_custom_command: Checks if the given command is a built-in shell command.
2. print_prompt: Prints the current shell prompt with custom formatting.
3. chprompt: Changes the shell prompt to a custom value.
4. my_getenv: Retrieves the value of an environment variable.
5. my_setenv: Sets an environment variable.
6. pwd: Returns the current working directory.
7. cd: Changes the current directory.
8. help: Displays help information about shell commands.
9. printenv: Prints all environment variables.
10. history: Displays the command history.
11. my_unsetenv: Unsets an environment variable.
12. echo: Prints a message or the value of an environment variable.
13. clear: Clears the terminal screen.
14. parser: Parses user input and executes commands.

📗 functions.h
This file defines constants, macros, and global variables used throughout the custom shell program. 

📗 my_lib.h
This file declares functions and global variables used throughout the custom shell program. It includes function prototypes related to environment management, shell operations, and input parsing. It also defines global variables and a structure for handling environment variables.

📗 shell.h
This file contains the basic configuration and declarations for the shell program. It primarily defines file paths for the environment and history files, initializes necessary file pointers, and defines the structure for environment variables.

📗  env.txt and history.txt
1. env.txt: Stores environment variable names and their values.
2. history.txt: Stores the history of commands executed in the shell.

📌 Compilation and Usage
1. Building the Project
2. Clone or download the project files.
3. Navigate to the project directory.
4. Run make to compile the project. This will create the customShell executable.
   make
5. Running the Program
   Once the project is compiled, you can run the shell with the following command:
   make run

📌 Available Commands
1. cd <path>: Change the current directory to the specified path.
2. pwd: Print the current working directory.
3. echo <message>: Print a message to the terminal.
4. history: Display the history of commands entered.
5. clear: Clear the terminal screen.
6. help <command>: Show help information for the specified command.
7. my_setenv <variable> <value>: Set an environment variable.
8. my_unsetenv <variable>: Unset an environment variable.
9. my_getenv <variable>: Get the value of an environment variable.
10. printenv: Print all environment variables.
11 chprompt <new_prompt>: Change the shell prompt.
