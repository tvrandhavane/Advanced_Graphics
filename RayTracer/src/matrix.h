#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

class matrix
{
    private:
    	int m;
    	int n;
    	vector<vector <float> > values;

    public:
        void set(int i,int j,float val);
    	matrix(int m, int n);
    	float at(int i, int j);
    	void multScalar(int c);
    	vector<float> multVector(vector<float> v);
        void multMatrix(matrix * mat);
        void print();

};

#endif // MATRIX_H