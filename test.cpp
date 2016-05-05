// #include "delta_cal.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include "log.h"

using namespace std;
// using namespace Log;


int main(){
	// Delta delta(400,400,600,600);
	// delta.inKin(Pos(3.33912,3.33905,-600.264));
	// delta.set_kin_theta(0,0,0);
	// delta.kin();
	// Delta::print_degree(delta.in_theta);
	// cout<<delta.in_theta[0]<<" "<<delta.in_theta[1]<<" "<<delta.in_theta[2]<<endl;
	// Pos::pos_print(delta.kin_pos);
	// struct tm *local;
	// time_t t = time(NULL);
	// local = localtime(&t);

	// cout<<local->tm_year+1900<<endl;
	// cout<<local->tm_mon+1<<endl;

	// cout<<ctime(&t)<<endl;;
	// ofstream f;
	// string s = "log.txt";
	// f.open(s.c_str(),ofstream::app);
	// f<<"haha"<<endl;
	// f.close();
	// Log::fileOpen();
	// cout<<Log::fileName<<endl;
	Log::log_d("test tag","msg");
}