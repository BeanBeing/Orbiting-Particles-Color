# Compiler
CC = g++

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# .cpp files
CPPFILES = *.cpp

# Compiler Flags
CFLAGS = -Wall -Wextra -Werror -pedantic

# Library Flags
LFLAGS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

# .obj files
OBJFILES = *.o

# Output
OUTPUT = app

.PHONY: app objects clean


app : objects
	mkdir -p $(BIN_DIR) ;$(CC) $(CFLAGS) $(LFLAGS) -o $(BIN_DIR)/$(OUTPUT) $(OBJ_DIR)/$(OBJFILES)

objects : $(SRC_DIR)/$(CPPFILES)
	mkdir -p $(OBJ_DIR) ;$(CC) $(CFLAGS)  -c $(SRC_DIR)/$(CPPFILES) $(LFLAGS) -o $(OBJ_DIR)/app.o

clean:
	rm -rf $(OBJ_DIR); rm -rf $(BIN_DIR)

