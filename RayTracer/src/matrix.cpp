#include "matrix.h"

matrix::matrix(int m, int n){
	values.resize(m, vector<float> (n, 0));
}

float matrix::at(int i, int j){
	return values[i][j];
}

void matrix::multScalar(int c){
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			values[i][j] *= c;
		}
	}
}