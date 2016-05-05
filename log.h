#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <iomanip>
#include "delta_cal.h"

class Log{
public:
	static void log_d(std::string tag,std::string msg);
	static void log_d(std::string tag,double value);
	static void log_d(std::string tag,Pos p);
	static void log_d(std::string tag,double *path,int pathNum);
	static string fileName;
	static ofstream fs;


private:
	static void fileOpen(){
		fs.open(fileName.c_str(),ofstream::app);
	}
	static void fileClose(){
		fs.close();
	}
	static char* timeStamp(){
		time_t t = time(NULL);
		return ctime(&t);
	}
};
