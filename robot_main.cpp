#include "delta_cau.h"
#include <iostream>

using namespace std;


const int ADD_BASE = 0x10000010;

int main(){

	float int x = *(ADD_BASE);
	float int y = *(ADD_BASE+0x4);
	float int z = *(ADD_BASE+0x8);

	
	Delta delta(400,400,600,600);
	Pos p1(100,200,-700);
	delta.inKin(p1);
	cout<<delta.in_theta[0]<<delta.in_theta[1]<<delta.in_theta[2]<<endl;
	delta.set_kin_theta(delta.in_theta[0],delta.in_theta[1],delta.in_theta[2]);
	delta.kin();
	Pos::pos_print(delta.kin_pos);
	return 0;

}