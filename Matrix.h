#pragma once


class Matrix {
public:
	double *matrix;
	int Rows, Cols; 
	Matrix(int, int);
	~Matrix();

};
void printMatrix(Matrix*, int, int);
void fillMatrix(Matrix&, int ,int);
double colMin(double*, int, int);
void transpon(double*, int *, int*);