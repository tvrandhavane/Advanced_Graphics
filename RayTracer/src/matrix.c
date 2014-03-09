#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

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

};

#endif // MATRIX_H