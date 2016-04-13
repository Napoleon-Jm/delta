#include "delta_cau.h"
#include <cmath>
#include <iostream>
#include "move_abs.h"


#define DEBUG_MOVE_ABS

ana_uni fromDigCodeToRad(dig_uni digCode,dig_uni codeBitNum){
	return digCode/pow(2,codeBitNum)*2*PI;
}

dig_uni fromRadToDigCode(ana_uni rad,dig_uni codeBitNum){
	return rad/2/PI*pow(2,codeBitNum);
}

/*
 *@Usage : generate a path for move absolute, if yor want to print
 *		   debug infomation, you need to define DEBUG_MOVE_ABS
 *
 *@parm x,y,z : target position
 *@parm axis1,axis2,axis3 : actual position
 *@parm maxVel : max speed
 *@parm maxAcc : max acceleration
 *@parm accPathNum : acceleration path size
 *@parm conVelPathNum : const velocity path size
 *
 *
 */
dig_uni* delta_move_abs_path(ana_uni x,ana_uni y,ana_uni z,
	dig_uni axis1,dig_uni axis2,dig_uni axis3,
	ana_uni maxVel,ana_uni maxAcc,
	dig_uni accPathNum,dig_uni conVelPathNum){

	Delta delta(400,400,600,600);

	delta.set_kin_theta(fromDigCodeToRad(axis1,13),fromDigCodeToRad(axis2,13),fromDigCodeToRad(axis3,13));

	
	delta.kin();

	#ifdef DEBUG_MOVE_ABS
	cout<<"***************** start point **********************"<<endl;
	Pos::pos_print(delta.kin_pos);
	cout<<"***************** *********** **********************"<<endl;
	#endif

	int path_size = 2*accPathNum+conVelPathNum;
	ana_uni *path = new ana_uni[path_size];
	dig_uni *dig_path = new dig_uni[path_size*3];
	ana_uni vel = 0.85*maxVel;
	ana_uni acc = 0.85*maxAcc;
	#ifdef DEBUG_MOVE_ABS
	cout<<"vel : "<<vel<<"  acc : "<<acc<<endl;
	#endif

	ana_uni t_acc = vel/acc;
	ana_uni t_del = t_acc/accPathNum;
	#ifdef DEBUG_MOVE_ABS
	cout<<"t_acc : "<<t_acc<<"  t_del : "<<t_del<<endl;
	#endif

	ana_uni distance = Pos::pos_norm(Pos::pos_sub(Pos(x,y,z),delta.kin_pos));
	Pos dis_unit = Pos::pos_mul(1/distance,Pos::pos_sub(Pos(x,y,z),delta.kin_pos));
	#ifdef DEBUG_MOVE_ABS
	cout<<"distance : "<<distance<<endl;

	cout<<"distance unit : ****************************"<<endl;
	Pos::pos_print(dis_unit);
	cout<<"distance unit : ****************************"<<endl;
	#endif

	// path[0] = t_del*t_del*acc/2;

	

	for(int i = 0;i<accPathNum;i++){
		path[i] = 0.5*acc*pow((i+1)*t_del,2);
	}
	#ifdef DEBUG_MOVE_ABS
	cout<<"acc t0 distance : "<<path[0]<<endl;
	#endif

	ana_uni distance_delta = (distance-t_acc*vel)/conVelPathNum;

	//total distance for acceleration period
	ana_uni distance_acc = path[accPathNum-1];
	#ifdef DEBUG_MOVE_ABS
	cout<<"distance_delta : "<<distance_delta<<"  distance_acc : "<<distance_acc<<endl;
	#endif

	for(int i = accPathNum;i<accPathNum+conVelPathNum;i++){
		path[i] = distance_acc+(i-accPathNum+1)*distance_delta;
	}
	//total distance for acceleration and const velocity period
	ana_uni distance_acc_vel = path[accPathNum+conVelPathNum-1];
	#ifdef DEBUG_MOVE_ABS
	cout<<"distance_acc_vel : "<<distance_acc_vel<<endl;
	#endif

	for(int i = accPathNum+conVelPathNum,j = accPathNum-1;i<path_size;i++){
		path[i] = path[i-1]+path[j]-path[j-1];
		j--;
	}


	for(int i = 0;i<path_size*3;){
		Pos p = Pos::pos_sum(delta.kin_pos,Pos::pos_mul(path[i],dis_unit));
		// cout<<p.x<<"  "<<p.y<<"  "<<p.z<<endl;
		delta.inKin( p );
		dig_path[i++] = delta.in_theta[0];
		dig_path[i++] = delta.in_theta[1];
		dig_path[i++] = delta.in_theta[3];
	}

	// Pos::pos_print(delta.kin_pos);
	#ifdef DEBUG_MOVE_ABS
	for(int i = 0;i < path_size;i++){
		Pos p = Pos::pos_sum(delta.kin_pos,Pos::pos_mul(path[i],dis_unit));
		cout<<p.x<<"  "<<p.y<<"  "<<p.z<<endl;
	}
	#endif
	delete[] path;

	return dig_path;

}


/*
 *test function
 */

int main(){
	
	float x = 400;
	float y = 400;
	float z = -600;
	float axis1 = PI/3;
	float axis2 = PI/3;
	float axis3 = PI/3;
	float maxVel = 10;
	float maxAcc = 1;
	dig_uni accPathNum = 3;
	dig_uni conVelPathNum = 4;
	unsigned int *test = delta_move_abs_path(x,y,z,fromRadToDigCode(axis1,13),fromRadToDigCode(axis2,13),fromRadToDigCode(axis3,13),maxVel,maxAcc,accPathNum,conVelPathNum);
	int path_size = (2*accPathNum + conVelPathNum)*3;
	// for(int i = 0;i < path_size;){
	// 	cout<<"x : "<<test[i++]<<"  y : "<<test[i++]<<"  z : "<<test[i++]<<endl;
	// }
}