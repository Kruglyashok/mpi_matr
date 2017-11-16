#include "mpi.h"
#include "stdio.h" 
#include "stdlib.h"
#include <ctime> 
#include "Matrix.h" 
#include <iostream>


using namespace std;

bool checkEq(double *cmins, double * mins, int num) 
{
	bool result = true; 
	for (int i = 0; i < num; i++)
	{ 
		if (cmins[i] != mins[i])
		{ 
			result = false; 
			break; 
		} 
	}
	return result; 
}

int main(int argc, char * argv[]) {
	
	int size, rank;
	Matrix *M; // start matrix
	double *mins; // mins for parallel part
	double *smins; // mins for sequential part
	double *buff; //each proc will recv cols in this buff
	double *min; // arr for mins in each proc

	int R = atoi(argv[1]); //Rows
	int C = atoi(argv[2]); //Cols

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		//creating data
		M = new Matrix(R, C);
		fillMatrix(*M, R, C);
		//printMatrix(M, R, C);
		cout << endl;

		smins = new double[C]; //prepare arr for sequential mins
		cout << "[Parallel Part] " << endl;
		//sequential part
		double begin_t1 = MPI_Wtime();

		transpon(M->matrix, &R, &C); //transponing matrix for sequential calculations
		//printMatrix(M, R, C);

		for (int i = 0; i < R; i++) {
			smins[i] = colMin(M->matrix, C, i);
		}
		double end_t1 = MPI_Wtime();
		end_t1 -= begin_t1;

		cout << "Sequential part work time: " << end_t1 << endl;

		//for (int i = 0; i < R; i++) {
		//	cout << "min in " << i << " col = " << smins[i] << endl;
		//}

		transpon(M->matrix, &R, &C); //transponing matrix back
	}

	//parallel part
	//start time 
	double begin_t = MPI_Wtime();
	int bSize = R*C / size; // block size, num of elements for each proc
	buff = new double[bSize];
	int block = bSize / R; // number of mins for each proc
	min = new double[block];
	mins = new double[C]; // creating final buff for parallel mins
	
	if (rank == 0) {
		cout << endl;
		cout << "[Parallel Part] " << endl;
		//printMatrix(M, R, C);

		transpon(M->matrix, &R, &C); //transponing matrix for parallel part
		//printMatrix(M, R, C);

		for (int j = 1; j < size; j++) {
			MPI_Send(&(*(M->matrix + bSize*j)), bSize, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
				}
		for (int i = 0; i < bSize; i++) {
			buff[i] = M->matrix[i];
		}
	}
	else {
		MPI_Recv(buff, bSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		
	}

	MPI_Bcast(&R, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&C, 1, MPI_INT, 0, MPI_COMM_WORLD);
	//      [Shared part]

	for (int i = 0; i < bSize / C; i++) {
		min[i] = colMin(buff, C, i);
	}
	
	// collecting data in root  proc
	MPI_Gather(min, block, MPI_DOUBLE, mins, block, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	
	// output of mins in root proc
	if (rank == 0) {
		double end_t = MPI_Wtime();

		end_t -= begin_t;
		cout << "Parallel part work time :" << end_t << endl;
		/*cout << endl;
		for (int i = 0; i < R; i++) {
			cout << "min in " << i << " col = " << mins[i] << endl;
		}*/
		if (checkEq(smins, mins, R) ) {
			cout << "Eq" << endl;
			 		}
		else {
			cout << "NotEq" << endl;
		}

	}

	delete mins, min, buff, smins, M;

	MPI_Finalize();
	return 0;
}