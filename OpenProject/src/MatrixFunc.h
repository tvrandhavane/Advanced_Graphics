#include "Matrix.h"

#include <iostream>

using namespace std;

class MatrixFunc{

public:
	MatrixFunc();
	int Size(const Matrix& a, const int i);
	Matrix Ones(const int rows, const int cols) ;
	Matrix Zeros(const int rows, const int cols);
	Matrix Diag(const int n);
	double Det(const Matrix& a);
	void Swap(double& a, double& b);
	Matrix Inv(const Matrix& a);
	Matrix Diag(const Matrix& v);
	Matrix Multiply(const Matrix & a, const Matrix & b);
};