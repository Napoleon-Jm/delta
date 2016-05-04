#include "tools.h"



void matrix_row_swap(m_uni matrix[][MATRIX_C_4],int r1,int r2,int columns = MATRIX_C_4){
	m_uni tmp = 0;
	for(int c = 0;c < columns;c++){
		tmp = matrix[r1][c];
		matrix[r1][c] = matrix[r2][c];
		matrix[r2][c] = tmp;
	}
}

void matrix_row_div(m_uni matrix[][MATRIX_C_4],int r,m_uni divisor,int columns = MATRIX_C_4){
	if(r<0||r>=MATRIX_R_3)
	{
		cout<<"matrix_row_div: r is out of range."<<r<<endl;
		return;
	} else if(divisor>-0.000001&&divisor<0.000001){
		cout<<"matrix_row_diy: divisor is 0. The matrix equation hasn't unique solution."<<endl;
	}
	for(int c = 0;c < columns;c++){
		matrix[r][c] = matrix[r][c]/divisor;
	}
}

void matrix_row_sub_k(m_uni matrix[][MATRIX_C_4],int r1,int r2,m_uni k,int columns = MATRIX_C_4){
	for(int c = 0;c < columns;c++){
		matrix[r1][c] -= matrix[r2][c]*k;
	}
}

void matrix_print(m_uni matrix[][MATRIX_C_4],int rows = MATRIX_R_3,int columns = MATRIX_C_4){
	cout<<"{"<<endl;
	for(int r = 0;r < rows;r++){
		cout<<"{ ";
		for(int c = 0;c < columns;c++){
			cout<<matrix[r][c];
			if(c!=columns-1)
				cout<<",";
			else
				cout<<" },"<<endl;
		}
	}
		cout<<"}"<<endl;;
}


void gaussianElimination(m_uni matrix[][MATRIX_C_4],int rows = MATRIX_R_3,int columns = MATRIX_C_4){
	int column = 0;
	int row = column;
	while(column < rows){
		row = column;
		if(matrix[row][column] == 0)
			for(int r = row+1;r < rows;r++){
				if(matrix[r][column] != 0){
					matrix_row_swap(matrix,row,r,columns);
					break;
				}
			}
		//matrix[row]/divisor
		matrix_row_div(matrix,row,matrix[row][column],columns);
		for(int r = row+1;r < rows;r++){
			m_uni first = matrix[r][column];
			if(first != 0)
				//r=r-row*k
				matrix_row_sub_k(matrix,r,row,first,columns);
		}
		column++;
		// cout<<"gaussian run."<<endl;
	}
	column = columns-2;
	row = rows - 2;
	while(row >= 0){
		for(int i = 0;i < (column-row);i++){
			matrix_row_sub_k(matrix,row,row+i+1,matrix[row][row+i+1]);
		}
		row--;
	}
}

void coordTransformationMatrix(m_uni T[][MATRIX_C_4],
		m_uni x0,m_uni y0,m_uni z0,
		m_uni x1,m_uni y1,m_uni z1,
		m_uni x2,m_uni y2,m_uni z2,
		m_uni x3,m_uni y3,m_uni z3,
		m_uni A1,m_uni B1,m_uni C1){

	m_uni l = sqrt(A1*A1+B1*B1+C1*C1);
	m_uni z_cosx = A1/l;
	m_uni z_cosy = B1/l;
	m_uni z_cosz = C1/l;
	Pos p0(x0,y0,z0);
	Pos p1(x1,y1,z1);
	l = Pos::pos_norm(Pos::pos_sub(p0,p1));

	m_uni x_cosx = (x1-x0)/l;
	m_uni x_cosy = (y1-y0)/l;
	m_uni x_cosz = (z1-z0)/l;

	m_uni y_cosx = z_cosy*x_cosz - z_cosz*x_cosy;
	m_uni y_cosy = z_cosz*x_cosx - z_cosx*x_cosz;
	m_uni y_cosz = z_cosx*x_cosy - z_cosy*x_cosx;

	T[0][0] = x_cosx;
	T[0][1] = y_cosx;
	T[0][2] = z_cosx;

	T[1][0] = x_cosy;
	T[1][1] = y_cosy;
	T[1][2] = z_cosy;

	T[2][0] = x_cosz;
	T[2][1] = y_cosz;
	T[2][2] = z_cosz;

	T[0][3] = x0;
	T[1][3] = y0;
	T[2][3] = z0;

	T[3][0] = T[3][1] = T[3][2] = 0;
	T[3][3] = 1;

}




int main(){

	// m_uni matrix[MATRIX_R_3][MATRIX_C_4] = {
	// 	{2,1,-1,8},
	// 	{-3,-1,2,-11},
	// 	{-2,1,2,-3},
	// };
	// matrix_print(matrix);
	// matrix_row_div(matrix,2,2);
	// matrix_print(matrix);
	// matrix_row_swap(matrix,0,1);
	// matrix_print(matrix);
	// matrix_row_sub_k(matrix,0,1,5);
	// matrix_print(matrix);

	// gaussianElimination(matrix);
	// matrix_print(matrix);
	m_uni T[MATRIX_R_4][MATRIX_C_4];


	Pos p1(0,0,-600);
	Pos p2(400,400,-600);
	Pos p3(800,0,-600);

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
	matrix_print(matrix,3,4);//print
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
	return 0;
}
