#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>

using namespace std;


#define MATRIX_R_3 3
#define MATRIX_C_4 4


typedef float m_uni;

void gaussianElimination(m_uni matrix[][MATRIX_C_4],int m,int n);
void matrix_row_swap(m_uni matrix[][MATRIX_C_4],int row,int r,int columns);
void matrix_row_div(m_uni matrix[][MATRIX_C_4],int r,m_uni num,int columns);
void matrix_row_sub_k(m_uni matrix[][MATRIX_C_4],int r,int row,m_uni first,int columns);
void matrix_print(m_uni matrix[][MATRIX_C_4],int rows,int columns);


#endif