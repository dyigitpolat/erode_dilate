all:
	g++ -std=c++11 plates.cpp `pkg-config --libs --cflags opencv` -o plate
	g++ -std=c++11 pets.cpp `pkg-config --libs --cflags opencv` -o pets