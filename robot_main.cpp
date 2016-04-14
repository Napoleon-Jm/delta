#include "delta_cal.h"
#include <iostream>

using namespace std;







//big laji laji laji laji laji laji laji
//PLC write algorithm read.
#define PLC_IS_WRITE 0;//0x00000
#define PLC_WRITE_COUNT 2//0X00002
#define ALG_IS_READ 14;//0x00014
#define ALG_READ_COUNT 16;//0x00016
#define ALG_READ_CONTENT 65536;//0x10000

//PLC read algorithm write.
#define PLC_IS_READ 262144;//0x40000
#define PLC_READ_COUNT 262156//0X40012
#define ALG_IS_WRITE 262158;//0X40014
#define ALG_WRITE_COUNT 262160;//0x40016
#define ALG_WRITE_CONTENT 327680;//0x50000








//base address of share memory
const int ADD_BASE = 0x10000010;

typedef float Dfloat;

//params of robot
const Dfloat R = 400;
const Dfloat r = 400;
const Dfloat l1 = 600;
const Dfloat l2 = 600;


//the number of path points. 
const int NUMBER_OF_PATH = 100;

//cycle time.
const int CYCLE_TIME = 1;//ms


//address to write path points.
#define VALUE_ADD 10000000;


int main(){

	//costruct robot delta.
	Delta delta(R,r,l1,l2);

	//target pos read from share memory.
	Dfloat x = *(ADD_BASE);
	Dfloat y = *(ADD_BASE+0x4);
	Dfloat z = *(ADD_BASE+0x8);
	//current degree read from share memory.
	Dfloat theta1 = *(ADD_BASE+0x12);
	Dfloat theta2 = *(ADD_BASE+0x16);
	Dfloat theta3 = *(ADD_BASE+0x20);

	delta.set_kin_theta(theta1,theta2,theta3);//current pos
	delta.inKin(Pos(x,y,z));//target degree

	//max speed read from share memory.
	Dfloat speed = *(ADD_BASE+0x24);


	//interval per path

	Dfloat theta1_interval = delta.in_theta[0]-theta1;
	Dfloat theta2_interval = delta.in_theta[1]-theta2;
	Dfloat theta3_interval = delta.in_theta[2]-theta3;


	//compare max speed.
	if(fabs(theta1_interval)/100/CYCLE_TIME > speed || 
		fabs(theta2_interval)/100/CYCLE_TIME > speed || 
		fabs(theta3_interval)/100/CYCLE_TIME > speed)
	{
		std::cout<<"max speed"<<std::endl;
		exit(-1);
	}

	//write path pos to VALUE_ADD in share memory.
	Dfloat *value = VALUE_ADD;
	Pos path[NUMBER_OF_PATH] = {0};


	for(int i = 1;i <= NUMBER_OF_PATH;i++){
		*value = delta.in_theta[0] + i*theta1_interval;
		value++;
		*value = delta.in_theta[1] + i*theta2_interval;
		value++;
		*value = delta.in_theta[2] + i*theta3_interval;
		value++;
	}

	
	return 0;
}
