GCC = gcc
SRC = shell.c ./src/functions.c
FILES = env.txt history.txt
EXE = customShell
DEL = rm

all:
	$(GCC) $(SRC) -o $(EXE)

run:
	./$(EXE)

delFiles:
	$(DEL) $(FILES)
	
del:
	$(DEL) $(EXE)
