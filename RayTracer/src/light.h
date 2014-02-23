#ifndef LIGHT_H
#define LIGHT_H
#include "color.h"
#include "Point.h"

class light
{
    public:
        light(Point * p, color * c);
        Point * location;
        color * lightcolor;
};

#endif // LIGHT_H
