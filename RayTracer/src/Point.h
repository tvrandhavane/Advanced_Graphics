#ifndef POINT_H
#define POINT_H

struct material_property{
	float kt;
	float kd;
	float ks;
	float kr;
    float n;
};

class Point
{
    public:
        Point();
        Point(float x1 , float y1 ,float z1);
        float x;
        float y;
        float z;

};

#endif // POINT_H
