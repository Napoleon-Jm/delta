delta:delta_cal.h robot_delta.cpp
	g++ -std=c++11 -D_REENTRANT -o delta robot_delta.cpp -lpthread -lglut -lGLU -lGL 
