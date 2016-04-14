#include "delta_cal.h"



int main(){
	Delta delta(400,400,600,600);
	delta.inKin(Pos(0,0,-600));
	delta.set_kin_theta(PI/3,PI/3,PI/3);
	delta.kin();
	Delta::print_degree(delta.in_theta);
	Pos::pos_print(delta.kin_pos);
}