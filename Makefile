CC = "g++"
SRC = src/main.cpp src/detector.cpp src/fps.cpp
PROJECT = "tubitak"
LIBS = `pkg-config --cflags --libs opencv4`

$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)
