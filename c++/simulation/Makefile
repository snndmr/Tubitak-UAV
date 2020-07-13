CC = "g++"
SRC = src/detector.cpp src/main.cpp  src/camera.cpp src/map.cpp 
PROJECT = "simulation"
LIBS = `pkg-config --cflags --libs opencv4`

$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)
