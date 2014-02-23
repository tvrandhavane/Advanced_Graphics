#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "ray.h"
#include "color.h"

class Triangle
{
    public:
        Triangle();
        Triangle(Point &temp_V1, Point &temp_V2, Point &temp_V3,material_property & pr, color * temp_c, int temp_type);
        float getValueOft(ray &r);
        vector<float> Normal;
        void getNormalAt(Point &P, ray & r, int dir);
        material_property property;
        int getType();
        color* getColor();
    protected:
    private:
        Point V1;
        Point V2;
        Point V3;
        int type;
        color *c;
        functions *funct;

        
};

#endif // TRIANGLE_H
