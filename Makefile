all:
	g++ -std=c++11 *.cpp `pkg-config --libs --cflags opencv` -o main
