#ifndef RAY_H
#define RAY_H
#include <iostream>
#include <vector>
#include <math.h>
#include "Point.h"
#include "functions.h"

using namespace std;
class ray
{
    public:
        ray();
        ray (float cos_x , float cos_y ,float cos_z ,Point *P);
        ray (float x1, float y1, float z1,float x2, float y2, float z2);
        vector <float> cosines;
        Point* origin;
        void initializeRay(Point &P1 , Point &P2);
        void initializeRay2(ray & r);
		int n;

    protected:
    private:
	   functions * funct;

};

#endif // RAY_H
