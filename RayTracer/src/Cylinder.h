#ifndef CYLINDER_H
#define CYLINDER_H
#include "ray.h"
#include "color.h"


class Cylinder {
    public:
        Cylinder();
        Cylinder(Point *BaseCenter, Point *TopCenter, float  rad, color* s_col, int type, material_property &p);
        float getRadius();
        void getNormalAt(Point &P, ray & r, int dir);
        float getValueOft(ray &r);
		color* getColor();
		Point* getBaseCenter();
        Point* getTopCenter();
        int getType();
        material_property property;
    protected:
    private:
        Point* baseCenter;
        Point* topCenter;
		color* s_color;
        float radius;
		functions *funct;
        int type;

};

#endif // CYLINDER_H
