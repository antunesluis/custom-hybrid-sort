APPS = ./apps
BIN = ./bin
INCLUDE = ./include
LIB = ./lib
OBJ = ./obj
SRC = ./src

FLAGS = -O3 -Wall
LIBS = -L$(LIB) -lmyproject -lm

all: directories generate myapps

directories:
	mkdir -p $(OBJ)
	mkdir -p $(BIN)
	mkdir -p $(LIB)

generate: $(OBJ)/file_utils.o \
	$(OBJ)/external_sort_utils.o \
	$(OBJ)/random_numbers.o \
	$(OBJ)/sorting_algorithms.o \
	$(OBJ)/cli.o
	ar -rcs $(LIB)/libmyproject.a $(OBJ)/*.o

myapps: clean_apps \
	$(BIN)/app

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	gcc $(FLAGS) -c $< -I$(INCLUDE) -o $@

$(BIN)/%: $(APPS)/%.c
	gcc $(FLAGS) $< $(LIBS) -I $(INCLUDE) -o $@

run:
	$(BIN)/app

clean:
	rm -rf $(BIN)/* $(LIB)/* $(OBJ)/*

clean_apps:
	rm -rf $(BIN)/*

