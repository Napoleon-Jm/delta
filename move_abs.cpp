#include "move_abs.h"


#define DEBUG_MOVE_ABS


//to calculate 2 pow of y.
dig_uni m_pow(dig_uni y){
	dig_uni x = 1;
	for(dig_uni i = 0;i < y;i++){
		x = x<<1;
	}
	return x;
}

ana_uni fromDigCodeToRad(dig_uni digCode,dig_uni codeBitNum){
	return ana_uni(digCode)/m_pow(codeBitNum)*2*PI;//ana_uni() is import.
}

dig_uni fromRadToDigCode(ana_uni rad,dig_uni codeBitNum){
	return rad/2/PI*m_pow(codeBitNum);
}

//Do not forget to delete the return pointer
dig_uni* fromRadsToDigCodes(ana_uni* rads,dig_uni len,dig_uni codeBitNum){
	dig_uni* dig_path = new dig_uni[len];
	for(int i = 0;i < len;i++){
		dig_path[i] = fromRadToDigCode(rads[i],codeBitNum);
	}
	return dig_path;
}

/*
 *@Usage : generate a path for move absolute, if yor want to print
 *		   debug infomation, you need to define DEBUG_MOVE_ABS
 *
 *@parm delta : the robot object delta.
 *@parm x,y,z : target position
 *@parm axis1,axis2,axis3 : actual position
 *@parm maxVel : max speed
 *@parm maxAcc : max acceleration
 *@parm accPathNum : acceleration path size
 *@parm conVelPathNum : const velocity path size
 *
 *NOTE : Do not forget delete the return pointer...
 */
ana_uni* delta_move_abs_path(Delta &delta,ana_uni x,ana_uni y,ana_uni z,
	dig_uni axis1,dig_uni axis2,dig_uni axis3,
	ana_uni maxVel,ana_uni maxAcc,
	dig_uni accPathNum,dig_uni conVelPathNum){

	// Delta delta(400,400,600,600);

	delta.set_kin_theta(fromDigCodeToRad(axis1,13),fromDigCodeToRad(axis2,13),fromDigCodeToRad(axis3,13));

	
	delta.kin();

	#ifdef DEBUG_MOVE_ABS
	cout<<"***************** start point **********************"<<endl;
	Pos::pos_print(delta.kin_pos);
	cout<<"***************** *********** **********************"<<endl;
	#endif

	int path_size = 2*accPathNum+conVelPathNum;
	ana_uni *path = new ana_uni[path_size];
	ana_uni *ana_path = new ana_uni[path_size*3];//debug the type is ana_uni, release the type is dig_uni
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


	for(int i = 0,j = 0;i<path_size*3;){
		Pos p = Pos::pos_sum(delta.kin_pos,Pos::pos_mul(path[j],dis_unit));
		cout<<p.x<<"  "<<p.y<<"  "<<p.z<<" in for "<<endl;
		delta.inKin( p );
		cout<<"in_theta in for"<<endl;
		cout<<delta.in_theta[0]<<" "<<delta.in_theta[1]<<" "<<delta.in_theta[2]<<endl;
		ana_path[i] = delta.in_theta[0];
		i++;
		ana_path[i] = delta.in_theta[1];
		i++;
		ana_path[i] = delta.in_theta[2];//fuck my fault.ana_path[i++] = delta.in_theta[3];
		i++;
		j++;
	}

	// Pos::pos_print(delta.kin_pos);
	#ifdef DEBUG_MOVE_ABS
	for(int i = 0;i < path_size;i++){
		Pos p = Pos::pos_sum(delta.kin_pos,Pos::pos_mul(path[i],dis_unit));
		cout<<p.x<<"  "<<p.y<<"  "<<p.z<<endl;
	}
	#endif
	delete[] path;

	return ana_path;

}

ana_uni* delta_move_abs_circle_path(Delta &delta,ana_uni x2,ana_uni y2,ana_uni z2,
	ana_uni x3,ana_uni y3,ana_uni z3,
	dig_uni axis1,dig_uni axis2,dig_uni axis3,
	dig_uni pathNum){

	ana_uni* ana_path = new ana_uni[pathNum*3];

	delta.set_kin_theta(fromDigCodeToRad(axis1,13),fromDigCodeToRad(axis2,13),fromDigCodeToRad(axis3,13));
	delta.kin();
	
	m_uni T[MATRIX_R_4][MATRIX_C_4];


	Pos p1(delta.kin_pos);
	Pos p2(x2,y2,z2);
	Pos p3(x3,y3,z3);

	m_uni A1 = (p1.y-p3.y)*(p2.z-p3.z)-(p2.y-p3.y)*(p1.z-p3.z);
	m_uni B1 = (p2.x-p3.x)*(p2.z-p3.z)-(p1.x-p3.x)*(p2.z-p3.z);
	m_uni C1 = (p1.x-p3.x)*(p2.y-p3.y)-(p2.x-p3.x)*(p1.y-p3.y);
	m_uni D1 = -( ( (p1.y-p3.y)*(p2.z-p3.z)-(p2.y-p3.y)*(p1.z-p3.z) )*p3.x + 
		( (p2.x-p3.y)*(p2.z-p3.z)-(p1.x-p3.y)*(p2.z-p3.z) )*p3.y + 
		( (p1.x-p3.x)*(p2.y-p3.y)-(p2.x-p3.x)*(p1.y-p3.y) )*p3.z );

	cout<<"A1 : "<<A1<<endl;
	cout<<"B1 : "<<B1<<endl;
	cout<<"C1 : "<<C1<<endl;
	cout<<"D1 : "<<D1<<endl;

	m_uni A2 = (p2.x-p1.x);
	m_uni B2 = (p2.y-p1.y);
	m_uni C2 = (p2.z-p1.z);
	m_uni D2 = -(Pos::pos_norm_2(p2)-Pos::pos_norm_2(p1))/2.0;

	m_uni A3 = (p3.x-p2.x);
	m_uni B3 = (p3.y-p2.y);
	m_uni C3 = (p3.z-p2.z);
	m_uni D3 = -(Pos::pos_norm_2(p3)-Pos::pos_norm_2(p2))/2.0;

	m_uni matrix[MATRIX_R_3][MATRIX_C_4] = {
		{A1,B1,C1,-D1},
		{A2,B2,C2,-D2},
		{A3,B3,C3,-D3},
	};




	gaussianElimination(matrix,3,4);
	matrix_print(matrix,3,4);
	coordTransformationMatrix(T,
		matrix[0][3],matrix[1][3],matrix[2][3],
		p1.x,p1.y,p1.z,
		p2.x,p2.y,p2.z,
		p3.x,p3.y,p3.z,
		A1,B1,C1);
	matrix_print(T,MATRIX_R_4,4);

	m_uni t03 = T[0][0]*matrix[0][3]+T[1][0]*matrix[1][3]+T[2][0]*matrix[2][3];
	m_uni t13 = T[0][1]*matrix[0][3]+T[1][1]*matrix[1][3]+T[2][1]*matrix[2][3];
	m_uni t23 = T[0][2]*matrix[0][3]+T[1][2]*matrix[1][3]+T[2][2]*matrix[2][3];


	m_uni T_inv[MATRIX_R_4][MATRIX_C_4] = {
		{T[0][0],T[1][0],T[2][0],t03},
		{T[0][1],T[1][1],T[2][1],t13},
		{T[0][2],T[1][2],T[2][2],t23},
		{0,0,0,1},
	};

	m_uni r = Pos::pos_norm(Pos::pos_sub(Pos(matrix[0][3],matrix[1][3],matrix[2][3]),p1));

	cout<<"r: "<<r<<endl;

	Pos p1_q(getTwoDimensionalPointFromThreeDiment(p1,T_inv));
	Pos p2_q(getTwoDimensionalPointFromThreeDiment(p2,T_inv));
	Pos p3_q(getTwoDimensionalPointFromThreeDiment(p3,T_inv));

	int pathNum1 = pathNum*(p2_q.x-p1_q.x)/(p3_q.x-p1_q.x);
	int pathNum2 = pathNum-pathNum1;

	int dir = (p2_q.y-p1_q.y)>0?1:-1;
	ana_uni delta_x = (p2_q.x-p1_q.x)/pathNum1;
	int index = 0;
	for(index = 0;index < pathNum1;){
		ana_path[index] = p1_q.x+delta_x*index;
		ana_path[index+1] = sqrt(r*r-ana_path[index]*ana_path[index])*dir;
		ana_path[index+2] = 0;
		index+=3;
	}

	delta_x = (p3_q.x-p2_q.x)/pathNum2;
	dir = (p3_q.y-p2_q.y)>0?1:-1;

	for(int i = index,j = 0;i<pathNum2;j++){
		ana_path[i] = p2_q.x+delta_x*j;
		ana_path[i+1] = sqrt(r*r-ana_path[i]*ana_path[i])*dir;
		ana_path[i+2] = 0;
		i+=3;
	}
	// Pos p1_q(-400,0,0);
	// Pos p2_q(0,400,0);
	// Pos p3_q(400,0,0);

	// Pos p1_p = getThreeDimensionalPointFromTwoDiment(p1_q,T);

	// Pos p2_p = getThreeDimensionalPointFromTwoDiment(p2_q,T);

	// Pos p3_p = getThreeDimensionalPointFromTwoDiment(p3_q,T);

	// Pos::pos_print(p1_p);
	// Pos::pos_print(p2_p);
	// Pos::pos_print(p3_p);
	return ana_path;
}


/*
 *test function
 */

int main(){
	// cout<<m_pow(5)<<endl;
	// float i = 1111111111.0f;
	// cout<<i<<endl;
	// cout<<fromDigCodeToRad(1,1)<<endl;

	Delta delta(400,400,600,600);
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
	ana_uni *test = delta_move_abs_path(delta,x,y,z,fromRadToDigCode(axis1,13),fromRadToDigCode(axis2,13),fromRadToDigCode(axis3,13),maxVel,maxAcc,accPathNum,conVelPathNum);
	int path_size = (2*accPathNum + conVelPathNum)*3;
	cout<<"--------------axis rad------------"<<endl;
	for(int i = 0;i < path_size;i = i+3){
		cout<<"axis1 : "<<test[i]<<"  axis2 : "<<test[i+1]<<"  axis3 : "<<test[i+2]<<endl;
	}
	dig_uni *dig_path = fromRadsToDigCodes(test,path_size,13);
	cout<<"--------------axis dig code------------"<<endl;
	for(int i = 0;i < path_size;i = i+3){
		cout<<"axis1 : "<<dig_path[i]<<"  axis2 : "<<dig_path[i+1]<<"  axis3 : "<<dig_path[i+2]<<endl;
	}


	delete[] test;
	delete[] dig_path;
}