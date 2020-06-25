CC = "g++"
PROJECT = "Tubitak"
SRC = Main.cpp Operation.cpp FPS.cpp
LIBS = `pkg-config --cflags --libs opencv4`

$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)