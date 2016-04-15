#include "delta_cal.h"



int main(){
	Delta delta(400,400,600,600);
	// delta.inKin(Pos(3.33912,3.33905,-600.264));
	delta.set_kin_theta(0,0,0);
	delta.kin();
	// Delta::print_degree(delta.in_theta);
	// cout<<delta.in_theta[0]<<" "<<delta.in_theta[1]<<" "<<delta.in_theta[2]<<endl;
	Pos::pos_print(delta.kin_pos);
}