#include "delta_cal.h"

#define DEBUG_DELTA_CAL

void Delta::inKin(Pos pos){
	Dfloat kuvs[3][3]={0.0};
	f_kuv(pos,kuvs);

	#ifdef DEBUG_DELTA_CAL
	cout<<"--------------kuv1--- "<<kuvs[0][0]<<" *** "<<kuvs[0][1]<<" *** "<<kuvs[0][2]<<endl;
	cout<<"--------------kuv2--- "<<kuvs[1][0]<<" *** "<<kuvs[1][1]<<" *** "<<kuvs[1][2]<<endl;
	cout<<"--------------kuv3--- "<<kuvs[2][0]<<" *** "<<kuvs[2][1]<<" *** "<<kuvs[2][2]<<endl;
	#endif

	Dfloat t;
	for(int i = 0;i<3;i++)
	{
		if(fabs(kuvs[i][0])<0.001)
			in_theta[i] = 90.0;
		else
		{
			t = (-kuvs[i][1]-sqrt(kuvs[i][1]*kuvs[i][1]-4*kuvs[i][0]*kuvs[i][2]))\
			/(2*kuvs[i][0]);
			in_theta[i] = fabs(2*atan(t));
		}
			

	}

}
void Delta::kin(){
	Pos v_OE[3];
	Pos PO_[3];
	Pos OE_[3];
	for(int i = 0;i<3;i++)
	{
		v_OE[i] = Pos( (R+l1*sin(kin_theta[i]))*cos(alpha[i]) , (R+l1*sin(kin_theta[i]))*sin(alpha[i]) , -l1*cos(kin_theta[i]) );
		PO_[i] = Pos( -r*cos(alpha[i]) , -r*sin(alpha[i]) , 0.0 );	
		OE_[i] = Pos::pos_sum(v_OE[i],PO_[i]); 

	}


	#ifdef DEBUG_DELTA_CAL
	for(auto i:v_OE)
	{
		cout<<"&&&&&&&&&&&&&&&  OE  "<<endl;
		Pos::pos_print(i);
	}


	for(auto i:OE_)
	{
		cout<<"&&&&&&&&&&&&&&&  OE_ "<<endl;
		Pos::pos_print(i);
	}
	#endif


	Pos OE = Pos::pos_mul(0.5,Pos::pos_sum(OE_[0],OE_[1]));//----------------------------------OE

	#ifdef DEBUG_DELTA_CAL
	cout<<"---OE---"<<endl;
	Pos::pos_print(OE);
	#endif

	Pos E_1E_2 = Pos::pos_sub(OE_[1],OE_[0]);
	Pos E_3E_1 = Pos::pos_sub(OE_[0],OE_[2]);
	Pos E_2E_3 = Pos::pos_sub(OE_[2],OE_[1]);

	#ifdef DEBUG_DELTA_CAL
	cout<<"---e1e2e3---"<<endl;
	Pos::pos_print(E_1E_2);
	Pos::pos_print(E_3E_1);
	Pos::pos_print(E_2E_3);
	#endif


	Dfloat a = Pos::pos_norm(E_1E_2);
	Dfloat b = Pos::pos_norm(E_3E_1);
	Dfloat c = Pos::pos_norm(E_2E_3);

	//theta0 = theta1 = theta2 == 0.
	if(a<0.00001 && b<0.00001 && c<0.00001){
		kin_pos = Pos(0,0,-(l1+l2));
		return;
	}

	//Debug
	// cout<<"---length of e1e2e3---"<<endl;
	// cout<<a<<"  "<<b<<"  "<<c<<endl;

	Dfloat E2F_NORM = (a*b*c)/( sqrt( (a+b+c)*(a+b-c)*(a+c-b)*(c+b-a) ) );

	
	Dfloat EF_NORM = sqrt(E2F_NORM*E2F_NORM-a*a*0.25);

	// cout<<"e2f_norm : "<<E2F_NORM<<endl;
	// cout<<"ef_norm : "<<EF_NORM<<endl;


	Dfloat cos_1 = Pos::pos_dot(E_2E_3,E_1E_2)/(c*a);
	Dfloat sin_1 = sqrt(1-cos_1*cos_1);

	Pos E_1E_2_cross_E_2E_3 = Pos::pos_mul(1.0/(a*c*sin_1), Pos::pos_cross(E_1E_2,E_2E_3) );


	// cout<<"e1e2  cross  e2e3 len : "<<Pos::pos_norm(E_1E_2_cross_E_2E_3)<<endl;// norm = 1

	// cout<<"e1e2  cross  e2e3 len     ---      dot   e1e2 "<<Pos::pos_dot(E_1E_2_cross_E_2E_3,E_1E_2)<<endl;


	Pos N_ef = Pos::pos_mul( 1.0/a , Pos::pos_cross( E_1E_2_cross_E_2E_3 , E_1E_2 ) );

	// cout<<"N_ef     LEN : "<<Pos::pos_norm(N_ef)<<endl;

	Pos EF = Pos::pos_mul(EF_NORM,N_ef);//-------------------------------------------------------EF

	Pos OF = Pos::pos_sum(OE,EF);//--------------------------------------------------------------OF

	#ifdef DEBUG_DELTA_CAL
	cout<<"N_ef vec is : "<<endl;
	Pos::pos_print(N_ef);

	cout<<"OF vec is : "<<endl;
	Pos::pos_print(OF);

	cout<<"RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR"<<endl;
	cout<<Pos::pos_norm(Pos::pos_sub(OF,OE_[0]))<<endl;
	cout<<Pos::pos_norm(Pos::pos_sub(OF,OE_[1]))<<endl;
	cout<<Pos::pos_norm(Pos::pos_sub(OF,OE_[2]))<<endl;
	#endif

	Dfloat FO_NORM = sqrt( l2*l2 - E2F_NORM*E2F_NORM);

	//Debug
	// cout<<"fo_ norm is : "<<FO_NORM<<endl;


	
	Pos N_fo_ = Pos::pos_mul( -1.0/(a*c*sin_1) , Pos::pos_cross(E_1E_2,E_2E_3) );

	//Debug
	#ifdef DEBUG_DELTA_CAL
	cout<<"e3e1 e1e2 cross is : "<<endl;
	Pos::pos_print(Pos::pos_cross(E_3E_1,E_1E_2));

	cout<<"cross .z * 1/(a*b) : "<<( -1.0/(a*b) )*Pos::pos_cross(E_3E_1,E_1E_2).z<<endl;

	cout<<"1/(a*b)"<<1/(a*b)<<endl;

	cout<<"N_fo_  LEN : "<<Pos::pos_norm(N_fo_)<<endl;;
	cout<<"N_fo_ vec is : "<<endl;
	Pos::pos_print(N_fo_);
	#endif

	Pos FO_ = Pos::pos_mul( FO_NORM , N_fo_ );


	#ifdef DEBUG_DELTA_CAL
	cout<<"FO_ vec is : "<<endl;
	Pos::pos_print(FO_);
	#endif



	kin_pos = Pos::pos_sum(OF,FO_);

	
}

void Delta::f_kuv(Pos pos,Dfloat kuv[3][3]){
	kuv[0][0] = l2*l2-l1*l1-pos.x*pos.x-pos.y*pos.y-pos.z*pos.z-(R-r)*(R-r)\
			+(R-r)*(1.732*pos.x+pos.y)+2*pos.z*l1;

	#ifdef DEBUG_DELTA_CAL
	Dfloat a = 2*pos.z*l1;
	// a = a/100.0;
	Dfloat b = l2*l2-l1*l1-pos.x*pos.x-pos.y*pos.y-pos.z*pos.z-(R-r)*(R-r)+(R-r)*(1.732*pos.x+pos.y);
	// b = b/100.0;
	cout<<"6666666666666666666666666: "<<2*pos.z*l1<<endl;
	cout<<"6666666666666666666666666: "<<(R-r)*(1.732*pos.x+pos.y)<<endl;
	cout<<"6666666666666666666666666: "<<(R-r)*(R-r)<<endl;
	cout<<"6666666666666666666666666: "<<pos.z*pos.z<<endl;
	cout<<"6666666666666666666666666: "<<pos.x*pos.x-pos.y*pos.y<<endl;
	cout<<"6666666666666666666666666: "<<l2*l2-l1*l1-pos.x*pos.x-pos.y*pos.y-pos.z*pos.z-(R-r)*(R-r)+(R-r)*(1.732*pos.x+pos.y)<<endl;
	cout<<"a:"<<a<<endl;
	cout<<"b:"<<b<<endl;
	cout<<"a+b: "<<a+b<<endl;
	cout<<"6666666666666666666666666: "<<kuv[0][0]<<endl;
	#endif

	kuv[0][1] = -2*(2*(R-r)-1.732*pos.x-pos.y)*l1;
	kuv[0][2] = kuv[0][0] - 4*pos.z*l1;

	kuv[1][0] = l2*l2-l1*l1-pos.x*pos.x-pos.y*pos.y-pos.z*pos.z-(R-r)*(R-r)\
			+(R-r)*(pos.y-1.732*pos.x)+2*pos.z*l1;
	kuv[1][1] = -2*(2*(R-r)+1.732*pos.x-pos.y)*l1;
	kuv[1][2] = kuv[1][0] - 4*pos.z*l1;

	kuv[2][0] = l2*l2-l1*l1-pos.x*pos.x-pos.z*pos.z-(R-r+pos.y)*(R-r+pos.y)\
			+2*pos.z*l1;
	kuv[2][1] = -4*l1*(R-r+pos.y);
	kuv[2][2] = kuv[2][0] - 4*pos.z*l1;

}

//test function for debug independent.
/*
int main(){

	
	// test for inkin
	Pos p1(0,0,-600);
	Delta delta1(400,400,600,600);
	delta1.inKin(p1);

	// cout<<"----------------main----------------"<<endl;

	// cout<<delta1.in_theta[0]<<delta1.in_theta[1]<<delta1.in_theta[2]<<endl;
	// return 0;
	

	//test for Pos operation
	// Pos p1(1,2,3),p2(5,5,5),p3;
	// Pos::pos_print(p3);
	// p3 = Pos::pos_sub(p2,p1);
	// Pos::pos_print(p3);
	// Pos::pos_print(Pos::pos_sum(p1,p2));
	// cout<<Pos::pos_len(p1)<<endl;
	// Pos ps[3];
	// ps[0] = Pos(1,1,1);
	// Pos::pos_print(ps[0]);
	

	//test for kin
	// Delta delta2(400,400,600,600);
	// delta2.set_kin_theta(delta1.in_theta[0]*PI/180,delta1.in_theta[1]*PI/180,delta1.in_theta[2]*PI/180);
	// delta2.kin();
	// cout<<"-----------------------"<<endl;
	// Pos::pos_print(delta2.kin_pos);
	// cout<<"-----------------------"<<endl;
	// cout<<delta1.in_theta[0]<<"   "<<delta1.in_theta[1]<<"   "<<delta1.in_theta[2]<<endl;
	// cout<<PI<<endl;
}
*/