CC = g++
STD = c++17
INCLUDE = ./include
SRC = $(wildcard ./src/*.cpp)
CFALGS = -Wall -Werror -Wextra -pedantic -O3

all: $(SRC)
	$(CC) -std=$(STD) $(CFALGS) -I$(INCLUDE) $(SRC) -o main