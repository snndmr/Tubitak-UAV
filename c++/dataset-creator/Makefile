CC = "g++"
SRC = src/main.cpp
PROJECT = "run"
LIBS = `pkg-config --cflags --libs opencv4`

$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)
