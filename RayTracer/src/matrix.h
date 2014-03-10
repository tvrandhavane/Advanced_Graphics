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
    	vector<float> postMultVector(vector<float> v);
        void preMultMatrix(matrix * mat);
        void postMultMatrix(matrix * mat);
        void print();
        void makeIdentity();
        matrix * transform();
};

#endif // MATRIX_H