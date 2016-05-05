#include "log.h"

std::string Log::fileName = "log.txt";
std::ofstream Log::fs;

void Log::log_d(std::string tag,std::string msg){
	fileOpen();

	fs<<timeStamp()<<'\t'<<'\t'<<tag<<" : "<<msg<<endl;
	
	fileClose();
}

void Log::log_d(std::string tag,double value){
	fileOpen();

	fs<<timeStamp()<<'\t'<<'\t'<<tag<<" : "<<value<<endl;
	
	fileClose();
}

void Log::log_d(std::string tag,Pos p){
	fileOpen();

	fs<<timeStamp()<<'\t'<<'\t'<<tag<<" : ";
	fs<<"( "<<p.x<<" ,"<<p.y<<" ,"<<p.z<<" )"<<endl;

	
	fileClose();
}

void Log::log_d(std::string tag,double *path,int pathNum){
	fileOpen();
	fs<<timeStamp();
	fs<<tag<<" : "<<endl;
	// fs.setprecision(6);
	for(int i = 0;i < pathNum*3;i+=3){
		fs<<fixed<<setw(6)<<'\t'<<'\t'<<"( "<<path[i]<<'\t'<<","<<path[i+1]<<'\t'<<","<<path[i+2]<<" )"<<endl;
	}
	fileClose();
}