#ifndef RAYINTERSECTION_H
#define RAYINTERSECTION_H
#include <iostream>
#include <vector>
#include "ray.h"

class rayIntersection
{
    public:
        rayIntersection();
        ray & getReflectedRay(Point &P,ray & r, ray & normal);
		ray & getRefractedRay(Point &P,float n1 , float n2 ,ray & incident_r,ray & normal);
    protected:
    private:
		functions * funct;
};
#endif // RAYINTERSECTION_H
