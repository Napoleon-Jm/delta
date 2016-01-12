#ifndef DELTA_CAU_H
#define DELTA_CAU_H


#include <cmath>
#include <iostream>


typedef float Dfloat;
using namespace std;

const Dfloat PI = 3.1415926;

struct Pos{
	Dfloat x;
	Dfloat y;
	Dfloat z;
	Pos():x(0.0),y(0.0),z(0.0){}
	Pos(Dfloat X,Dfloat Y,Dfloat Z):x(X),y(Y),z(Z){}
	static Pos pos_sub(Pos p1,Pos p2)
	{
		return Pos(p1.x-p2.x,p1.y-p2.y,p1.z-p2.z);
	}
	static Pos pos_sum(Pos p1,Pos p2)
	{
		return Pos(p1.x+p2.x,p1.y+p2.y,p1.z+p2.z);
	}
	static Dfloat pos_norm(Pos p)
	{
		return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
	}
	static Dfloat pos_norm_2(Pos p)
	{
		return p.x*p.x+p.y*p.y+p.z*p.z;
	}
	static Pos pos_cross(Pos p1,Pos p2)
	{
		return Pos(p1.y*p2.z-p1.z*p2.y,p1.z*p2.x-p1.x*p2.z,p1.x*p2.y-p1.y*p2.x);
	}
	static Dfloat pos_dot(Pos p1,Pos p2)
	{
		return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
	}
	static Pos pos_mul(Dfloat k,Pos p)
	{
		return Pos(k*p.x,k*p.y,k*p.z);
	}
	static void pos_print(Pos p)
	{
		cout<<"x = "<<p.x<<endl;
		cout<<"y = "<<p.y<<endl;
		cout<<"z = "<<p.z<<endl;
	}
};

struct Delta{
	Dfloat R;
	Dfloat r;
	Dfloat alpha[3];
	Dfloat l1;
	Dfloat l2;
	//use befor kin
	Dfloat kin_theta[3];
	Pos kin_pos;
	//use befor in_kin function
	Dfloat in_theta[3];
	//E and P for draw l2
	Pos E[3];
	Pos P[3];
	Delta(Dfloat R_,Dfloat r_,Dfloat l1_,Dfloat l2_):R(R_),r(r_),l1(l1_),l2(l2_),kin_pos(Pos(0,0,0)){
		for(int i = 0;i<3;i++)
		{
			alpha[i] = (4*(i+1)-3)*PI/6.0;
		}
	}
	void set_kin_theta(Dfloat theta1,Dfloat theta2,Dfloat theta3){
		kin_theta[0] = theta1;
		kin_theta[1] = theta2;
		kin_theta[2] = theta3;
	}
	void f_kuv(Pos pos,Dfloat kuv[3][3]){
		kuv[0][0] = l2*l2-l1*l1-pos.x*pos.x-pos.y*pos.y-pos.z*pos.z-(R-r)*(R-r)\
				+(R-r)*(1.732*pos.x+pos.y)+2*pos.z*l1;

		// Dfloat a = 2*pos.z*l1;
		// // a = a/100.0;
		// Dfloat b = l2*l2-l1*l1-pos.x*pos.x-pos.y*pos.y-pos.z*pos.z-(R-r)*(R-r)+(R-r)*(1.732*pos.x+pos.y);
		// // b = b/100.0;
		// cout<<"6666666666666666666666666: "<<2*pos.z*l1<<endl;
		// cout<<"6666666666666666666666666: "<<(R-r)*(1.732*pos.x+pos.y)<<endl;
		// cout<<"6666666666666666666666666: "<<(R-r)*(R-r)<<endl;
		// cout<<"6666666666666666666666666: "<<pos.z*pos.z<<endl;
		// cout<<"6666666666666666666666666: "<<pos.x*pos.x-pos.y*pos.y<<endl;
		// cout<<"6666666666666666666666666: "<<l2*l2-l1*l1-pos.x*pos.x-pos.y*pos.y-pos.z*pos.z-(R-r)*(R-r)+(R-r)*(1.732*pos.x+pos.y)<<endl;
		// cout<<"a:"<<a<<endl;
		// cout<<"b:"<<b<<endl;
		// cout<<"a+b: "<<a+b<<endl;
		// cout<<"6666666666666666666666666: "<<kuv[0][0]<<endl;

		kuv[0][1] = -2*(2*(R-r)-1.732*pos.x-pos.y)*l1;
		kuv[0][2] = kuv[0][0] - 4*pos.z*l1;
		// return kuv1;
	// }
	// void f_kuv2(Pos pos,Dfloat kuv[]){
		kuv[1][0] = l2*l2-l1*l1-pos.x*pos.x-pos.y*pos.y-pos.z*pos.z-(R-r)*(R-r)\
				+(R-r)*(pos.y-1.732*pos.x)+2*pos.z*l1;
		kuv[1][1] = -2*(2*(R-r)+1.732*pos.x-pos.y)*l1;
		kuv[1][2] = kuv[1][0] - 4*pos.z*l1;
		// return kuv1;
	// }
	// void f_kuv3(Pos pos,Dfloat kuv[]){
		kuv[2][0] = l2*l2-l1*l1-pos.x*pos.x-pos.z*pos.z-(R-r+pos.y)*(R-r+pos.y)\
				+2*pos.z*l1;
		kuv[2][1] = -4*l1*(R-r+pos.y);
		kuv[2][2] = kuv[2][0] - 4*pos.z*l1;
		// return kuv;
	}

	Pos inKin(Pos pos){
		Dfloat kuvs[3][3]={0.0};
		f_kuv(pos,kuvs);
		
		cout<<"--------------kuv1--- "<<kuvs[0][0]<<" *** "<<kuvs[0][1]<<" *** "<<kuvs[0][2]<<endl;
		cout<<"--------------kuv2--- "<<kuvs[1][0]<<" *** "<<kuvs[1][1]<<" *** "<<kuvs[1][2]<<endl;
		cout<<"--------------kuv3--- "<<kuvs[2][0]<<" *** "<<kuvs[2][1]<<" *** "<<kuvs[2][2]<<endl;
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

	Pos kin(){
		Pos v_OE[3];
		Pos PO_[3];
		Pos OE_[3];
		for(int i = 0;i<3;i++)
		{
			v_OE[i] = Pos( (R+l1*sin(kin_theta[i]))*cos(alpha[i]) , (R+l1*sin(kin_theta[i]))*sin(alpha[i]) , -l1*cos(kin_theta[i]) );
			PO_[i] = Pos( -r*cos(alpha[i]) , -r*sin(alpha[i]) , 0.0 );	
			OE_[i] = Pos::pos_sum(v_OE[i],PO_[i]); 

		}

		for(auto i:v_OE)
		{
			cout<<"&&&&&&&&&&&&&&&  OE  "<<endl;
			Pos::pos_print(i);
		}

		//Debug
		for(auto i:OE_)
		{
			cout<<"&&&&&&&&&&&&&&&  OE_ "<<endl;
			Pos::pos_print(i);
		}

		Pos OE = Pos::pos_mul(0.5,Pos::pos_sum(OE_[0],OE_[1]));//----------------------------------OE
		//Debug
		cout<<"---OE---"<<endl;
		Pos::pos_print(OE);
		Pos E_1E_2 = Pos::pos_sub(OE_[1],OE_[0]);
		Pos E_3E_1 = Pos::pos_sub(OE_[0],OE_[2]);
		Pos E_2E_3 = Pos::pos_sub(OE_[2],OE_[1]);

		//Debug
		cout<<"---e1e2e3---"<<endl;
		Pos::pos_print(E_1E_2);
		Pos::pos_print(E_3E_1);
		Pos::pos_print(E_2E_3);


		Dfloat a = Pos::pos_norm(E_1E_2);
		Dfloat b = Pos::pos_norm(E_3E_1);
		Dfloat c = Pos::pos_norm(E_2E_3);

		//Debug
		cout<<"---length of e1e2e3---"<<endl;
		cout<<a<<"  "<<b<<"  "<<c<<endl;

		Dfloat E2F_NORM = (a*b*c)/( sqrt( (a+b+c)*(a+b-c)*(a+c-b)*(c+b-a) ) );

		
		Dfloat EF_NORM = sqrt(E2F_NORM*E2F_NORM-a*a*0.25);

		cout<<"e2f_norm : "<<E2F_NORM<<endl;
		cout<<"ef_norm : "<<EF_NORM<<endl;


		Dfloat cos_1 = Pos::pos_dot(E_2E_3,E_1E_2)/(c*a);
		Dfloat sin_1 = sqrt(1-cos_1*cos_1);

		Pos E_1E_2_cross_E_2E_3 = Pos::pos_mul(1.0/(a*c*sin_1), Pos::pos_cross(E_1E_2,E_2E_3) );


		cout<<"e1e2  cross  e2e3 len : "<<Pos::pos_norm(E_1E_2_cross_E_2E_3)<<endl;// norm = 1

		cout<<"e1e2  cross  e2e3 len     ---      dot   e1e2 "<<Pos::pos_dot(E_1E_2_cross_E_2E_3,E_1E_2)<<endl;


		Pos N_ef = Pos::pos_mul( 1.0/a , Pos::pos_cross( E_1E_2_cross_E_2E_3 , E_1E_2 ) );

		cout<<"N_ef     LEN : "<<Pos::pos_norm(N_ef)<<endl;

		Pos EF = Pos::pos_mul(EF_NORM,N_ef);//-------------------------------------------------------EF

		Pos OF = Pos::pos_sum(OE,EF);//--------------------------------------------------------------OF

		//Debug
		cout<<"N_ef vec is : "<<endl;
		Pos::pos_print(N_ef);

		cout<<"OF vec is : "<<endl;
		Pos::pos_print(OF);

		cout<<"RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR"<<endl;
		cout<<Pos::pos_norm(Pos::pos_sub(OF,OE_[0]))<<endl;
		cout<<Pos::pos_norm(Pos::pos_sub(OF,OE_[1]))<<endl;
		cout<<Pos::pos_norm(Pos::pos_sub(OF,OE_[2]))<<endl;

		Dfloat FO_NORM = sqrt( l2*l2 - E2F_NORM*E2F_NORM);

		//Debug
		cout<<"fo_ norm is : "<<FO_NORM<<endl;


		
		Pos N_fo_ = Pos::pos_mul( -1.0/(a*c*sin_1) , Pos::pos_cross(E_1E_2,E_2E_3) );

		//Debug

		// cout<<"e3e1 e1e2 cross is : "<<endl;
		// Pos::pos_print(Pos::pos_cross(E_3E_1,E_1E_2));

		// cout<<"cross .z * 1/(a*b) : "<<( -1.0/(a*b) )*Pos::pos_cross(E_3E_1,E_1E_2).z<<endl;

		// cout<<"1/(a*b)"<<1/(a*b)<<endl;

		cout<<"N_fo_  LEN : "<<Pos::pos_norm(N_fo_)<<endl;;
		cout<<"N_fo_ vec is : "<<endl;
		Pos::pos_print(N_fo_);

		Pos FO_ = Pos::pos_mul( FO_NORM , N_fo_ );


		//Debug
		cout<<"FO_ vec is : "<<endl;
		Pos::pos_print(FO_);



		kin_pos = Pos::pos_sum(OF,FO_);

		
	}

	void calculate_l2_endpoint(Dfloat *theta,Pos p){
		for(int i = 0;i < 3;i++){
			E[i] = Pos((R+l1*sin(theta[i]))*cos(alpha[i]) , (R+l1*sin(theta[i]))*sin(alpha[i]) , -l1*cos(theta[i]));
		}
		for(int i = 0;i < 3;i++)
		{
			P[i] = Pos(r*cos(alpha[i])+p.x , r*sin(alpha[i])+p.y , p.z);
		}
	}

	void len_l2(Dfloat *theta,Pos p,Dfloat *lenL2)
	{
		calculate_l2_endpoint(theta,p);

		for(int i = 0;i < 3;i++)
		{
			lenL2[i] = Pos::pos_norm(Pos::pos_sub(P[i],E[i]));
		}
	}

	static void print_degree(Dfloat *d){
		cout<<"Degree -----------------------"<<endl;
		cout<<d[0]*180/PI<<" "<<d[1]*180/PI<<" "<<d[2]*180/PI<<endl;
		cout<<"Degree -----------------------"<<endl;
	}
};

#endif


/*
int main(){

	
	// test for inkin
	Pos p1(100,200,-700);
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
	Delta delta2(400,400,600,600);
	delta2.set_kin_theta(delta1.in_theta[0]*PI/180,delta1.in_theta[1]*PI/180,delta1.in_theta[2]*PI/180);
	delta2.kin();
	cout<<"-----------------------"<<endl;
	Pos::pos_print(delta2.kin_pos);
	cout<<"-----------------------"<<endl;
	cout<<delta1.in_theta[0]<<"   "<<delta1.in_theta[1]<<"   "<<delta1.in_theta[2]<<endl;
	// cout<<PI<<endl;
}
*/