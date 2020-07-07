CC = "g++"
SRC = src/main.cpp src/detector.cpp src/fps.cpp #src/sonar.cpp
PROJECT = "tubitak"
LIBS = `pkg-config --cflags --libs opencv4` #-lwiringPi

$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)
