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




int main(){

	m_uni matrix[MATRIX_R_3][MATRIX_C_4] = {
		{2,1,-1,8},
		{-3,-1,2,-11},
		{-2,1,2,-3},
	};
	// matrix_print(matrix);
	// matrix_row_div(matrix,2,2);
	// matrix_print(matrix);
	// matrix_row_swap(matrix,0,1);
	// matrix_print(matrix);
	// matrix_row_sub_k(matrix,0,1,5);
	// matrix_print(matrix);

	gaussianElimination(matrix);
	matrix_print(matrix);
	return 0;
}
