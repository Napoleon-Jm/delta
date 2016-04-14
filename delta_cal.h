#ifndef DELTA_CAU_H
#define DELTA_CAU_H


#include <cmath>
#include <iostream>


// #define DEBUG_DELTA_CAL

//the type for delta_cau , you can change it if you need.
typedef float Dfloat;
using namespace std;

//the const variable for PI.
//the cmath pi is not stable for using.
const Dfloat PI = 3.1415926;


/*
 *@Usage : Pos representation, include some operations for points
 *       like, sub, sum, cross, dot, and so on.
 *
 *@Author : Wang Jimin
 *@email : wangjimin.c@hotmail.com
 *
 */
struct Pos{
	Dfloat x;
	Dfloat y;
	Dfloat z;
	//constructor
	Pos():x(0.0),y(0.0),z(0.0){}
	Pos(Dfloat X,Dfloat Y,Dfloat Z):x(X),y(Y),z(Z){}
	//static function for points subtraction.
	static Pos pos_sub(Pos p1,Pos p2)
	{
		return Pos(p1.x-p2.x,p1.y-p2.y,p1.z-p2.z);
	}
	//static function for points sum.
	static Pos pos_sum(Pos p1,Pos p2)
	{
		return Pos(p1.x+p2.x,p1.y+p2.y,p1.z+p2.z);
	}
	//static function for point modular.
	static Dfloat pos_norm(Pos p)
	{
		return sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
	}
	//static function for point modular square.
	static Dfloat pos_norm_2(Pos p)
	{
		return p.x*p.x+p.y*p.y+p.z*p.z;
	}
	//static function for vector cross product.
	static Pos pos_cross(Pos p1,Pos p2)
	{
		return Pos(p1.y*p2.z-p1.z*p2.y,p1.z*p2.x-p1.x*p2.z,p1.x*p2.y-p1.y*p2.x);
	}
	//static function for vector dot product.
	static Dfloat pos_dot(Pos p1,Pos p2)
	{
		return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
	}
	//static function for vector multiplication.
	static Pos pos_mul(Dfloat k,Pos p)
	{
		return Pos(k*p.x,k*p.y,k*p.z);
	}
	//static function for print points to dubug.
	static void pos_print(Pos p)
	{
		cout<<"x = "<<p.x<<endl;
		cout<<"y = "<<p.y<<endl;
		cout<<"z = "<<p.z<<endl;
	}
};
/*
 *@Usage : Delta robot representation class, include some import function
 *       like, kin and inkin kinematics algorithms.
 *@Author : Wang Jimin
 *@email : wangjimin.c@hotmail.com
 *
 *@Debeg : you can print debug info to define DEBUG_DELTA_CAL
 */
struct Delta{
	/*
	 *parm for delta robot,R,r,l1,l2.
	*/
	Dfloat R;
	Dfloat r;
	Dfloat alpha[3];
	Dfloat l1;
	Dfloat l2;
	//need to be set befor kin function.
	Dfloat kin_theta[3];
	Pos kin_pos;
	//its result for inKin function.
	Dfloat in_theta[3];
	//E and P for draw l2
	Pos E[3];
	Pos P[3];
	//constructor
	Delta(Dfloat R_,Dfloat r_,Dfloat l1_,Dfloat l2_):R(R_),r(r_),l1(l1_),l2(l2_),kin_pos(Pos(0,0,0)){
		for(int i = 0;i<3;i++)
		{
			alpha[i] = (4*(i+1)-3)*PI/6.0;
		}
	}
	//set kin theta for kin function.
	void set_kin_theta(Dfloat theta1,Dfloat theta2,Dfloat theta3){
		kin_theta[0] = theta1;
		kin_theta[1] = theta2;
		kin_theta[2] = theta3;
	}
	//its called by kin function.
	void f_kuv(Pos pos,Dfloat kuv[3][3]);
	
	/*
	 *@Usage:inverse kinematics
	 *
	 *@parm : pos,the target position for inverse kinematics.
	 *@result : the result of inverse kinematics function is saved in in_theta.
	*/
	 void inKin(Pos pos);
	
	/*
	 *@Usage:forward kinematics function
	 *
	 *@result:the forward kinematics fuction result is saved in kin_pos.
	*/
	 void kin();
	

	/*
	 *@Usage : calculate E and P vector
	 *
	 *@pram : theta, the degree theta between l1 and R.
	 *@pram : p, the end point of delta, to calculate P.
	 *
	 *@result : saved in P and E.
	*/

	void calculate_l2_endpoint(Dfloat *theta,Pos p){
		for(int i = 0;i < 3;i++){
			E[i] = Pos((R+l1*sin(theta[i]))*cos(alpha[i]) , (R+l1*sin(theta[i]))*sin(alpha[i]) , -l1*cos(theta[i]));
		}
		for(int i = 0;i < 3;i++)
		{
			P[i] = Pos(r*cos(alpha[i])+p.x , r*sin(alpha[i])+p.y , p.z);
		}
	}
	/*
	 *@Usage : calculate length of l2, maybe used for verifing the algorithm.
	 *
	 *@pram : theta, for function calculate_l2_endpoint.
	 *@parm : p, for function calculate_l2_endpoint.
	 *@pram : lenL2, used for save result.
	 *
	*/
	void len_l2(Dfloat *theta,Pos p,Dfloat *lenL2)
	{
		calculate_l2_endpoint(theta,p);

		for(int i = 0;i < 3;i++)
		{
			lenL2[i] = Pos::pos_norm(Pos::pos_sub(P[i],E[i]));
		}
	}
	//for debug to print degree, d is rad measure,print is degree measure.
	static void print_degree(Dfloat *d){
		cout<<"Degree -----------------------"<<endl;
		cout<<d[0]*180/PI<<" "<<d[1]*180/PI<<" "<<d[2]*180/PI<<endl;
		cout<<"Degree -----------------------"<<endl;
	}
};

#endif



