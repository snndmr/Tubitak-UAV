CC = "g++"
SRC = src/main.cpp src/operation.cpp src/fps.cpp
PROJECT = "Tubitak"
LIBS = `pkg-config --cflags --libs opencv4`

$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)