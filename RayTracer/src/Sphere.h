#ifndef SPHERE_H
#define SPHERE_H
#include "ray.h"
#include "color.h"


class Sphere {
    public:
        Sphere();
        Sphere(Point *Center, float  rad, color* s_col, int type, material_property &p);
        float getRadius();
        void getNormalAt(Point &P, ray & r, int dir);
        float getValueOft(ray &r);
		color* getColor();
		Point* getCenter();
        int getType();
        material_property property;
    protected:
    private:
        Point* center;
		color* s_color;
        float radius;
		functions *funct;
        int type;

};

#endif // SPHERE_H
