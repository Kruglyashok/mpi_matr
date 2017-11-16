#include "Matrix.h"
#include <iostream>
#include <ctime>

using namespace std;

void transpon(double *matrix, int *rs, int *cs) {
	double *result;
	int rows = *rs;
	int cols = *cs;
	result = new double[rows*cols];
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			result[i*rows + j] = matrix[i + j*cols];
		}
	}
	for (int i = 0; i < rows*cols; i++) {
		matrix[i] = result[i];
	}
	*rs = cols;
	*cs = rows;

}


void printMatrix(Matrix *M, int rows, int cols) {
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << M->matrix[j + i*cols] << '\t';
		}
		cout << endl;
	}

}

void fillMatrix(Matrix &M, int rows, int cols) {
	srand(time(0));
	for (int i = 0; i < cols*rows; i++) { 
		M.matrix[i] =i;
		//M.matrix[i] = rand() % 2000 - 1000;

			}
}

Matrix::Matrix(int rows, int cols) {
	Rows = rows;
	Cols = cols;
	matrix = new double[rows*cols];
}

Matrix::~Matrix()
{
	delete matrix;
}

double colMin(double *matrix, int rows, int colNum) {
	double min = matrix[colNum*rows];
	for (int i = 0; i < rows; i++) {
		if (min > matrix[i + colNum*rows]) {
			min = matrix[i + colNum*rows];
		}
	}
	return min;

}