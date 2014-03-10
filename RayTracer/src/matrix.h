#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

using namespace std;

class matrix
{
    private:
    	int m;
    	int n;
    	vector<vector <float> > values;

    public:
    	matrix(int m, int n);
    	float at(int i, int j);
    	void multScalar(int c);
    	vector<float> multVector(vector<float> v);

};

#endif // MATRIX_H