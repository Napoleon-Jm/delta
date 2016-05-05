delta:delta_cal.h robot_delta.cpp
	g++ -std=c++11 -D_REENTRANT -o delta robot_delta.cpp -lpthread -lglut -lGLU -lGL 


move_abs.cpp
	g++ -o move move_abs.cpp delta_cal.cpp tools.cpp log.cpp
