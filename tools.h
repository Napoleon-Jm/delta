#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <cmath>
#include "delta_cal.h"

using namespace std;


#define MATRIX_R_3 3
#define MATRIX_R_4 4
#define MATRIX_C_4 4


typedef float m_uni;

void gaussianElimination(m_uni matrix[][MATRIX_C_4],int m,int n);
void matrix_row_swap(m_uni matrix[][MATRIX_C_4],int row,int r,int columns);
void matrix_row_div(m_uni matrix[][MATRIX_C_4],int r,m_uni num,int columns);
void matrix_row_sub_k(m_uni matrix[][MATRIX_C_4],int r,int row,m_uni first,int columns);
void matrix_print(m_uni matrix[][MATRIX_C_4],int rows,int columns);

void coordTransformationMatrix(m_uni T[][MATRIX_C_4],
		m_uni x0,m_uni y0,m_uni z0,
		m_uni x1,m_uni y1,m_uni z1,
		m_uni x2,m_uni y2,m_uni z2,
		m_uni x3,m_uni y3,m_uni z3,
		m_uni A1,m_uni B1,m_uni C1);

void getT_inv(m_uni T[][MATRIX_C_4],m_uni T_inv[][MATRIX_C_4]);

Pos TMultiP(m_uni T[][MATRIX_C_4],Pos p);//T*(p,1)


#endif