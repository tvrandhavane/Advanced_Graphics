#include "matrix.h"

matrix::matrix(int m, int n){
	this->m = m;
	this->n = n;
	values.resize(m, vector<float> (n, 0));
}

float matrix::at(int i, int j){
	return values[i][j];
}

void matrix::set(int i,int j,float val){
	values[i][j] = val;
}

void matrix::multScalar(int c){
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			values[i][j] *= c;
		}
	}
}

vector<float> matrix::postMultVector(vector<float> v){
	if(n != v.size()){
		cout << "Error: Vector and matrix sizes don't match." << endl;
		exit(1);
	}
	vector<float> toReturn(m, 0);
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			toReturn[i] += v[j]*values[i][j];
		}
	}
	return toReturn;
}

void matrix::postMultMatrix(matrix * mat){
	if((this->n != mat->m) || (mat->m != mat->n)){
		cout << "Error: Matrix sizes do not match. " << endl;
		exit(1);
	}
	matrix newm = matrix(this->m, this->n);
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			int val = 0;
			for(int k = 0; k < n; k++){
				val += values[i][k]*mat->at(k, j);
			}
			newm.set(i,j,val);
		}
	}
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			values[i][j] = newm.at(i, j);
		}
	}
}

void matrix::preMultMatrix(matrix * mat){
	if((this->m != mat->m) || (mat->m != mat->n)){
		cout << "Error: Matrix sizes do not match. " << endl;
		exit(1);
	}
	matrix newm = matrix(this->m, this->n);
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			int val = 0;
			for(int k = 0; k < n; k++){
				val += values[k][j]*mat->at(i, k); //k, j
			}
			newm.set(i,j,val);
		}
	}
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			values[i][j] = newm.at(i, j);
		}
	}
}

void matrix::print(){
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			cout << values[i][j] << " ";
		}
		cout << endl;
	}
}

void matrix::makeIdentity(){
	if(m != n){
		cout << "Matrix sizes don't match." << endl;
		exit(1);
	}
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			values[i][j] = 0;
		}
	}
	for(int i = 0; i < m; i++){
		values[i][i] = 1;
	}
}

matrix * matrix::transform(){
	matrix * transformed =  new matrix(this->n, this->m);

	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			transformed->set(j, i, values[i][j]);
		}
	}

	return transformed;
}