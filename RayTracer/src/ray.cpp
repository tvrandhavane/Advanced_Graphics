#include "ray.h"

ray::ray()
{
    cosines.resize(3);
    cosines[0]=0;
    cosines[1]=0;
    cosines[2]=0;
    origin= new Point(0.0,0.0,0.0);
	funct= new functions();
    n = 1;

}

ray::ray(float cos_x , float cos_y ,float cos_z ,Point *P){
    cosines.resize(3);
    cosines[0]=cos_x;
    cosines[1]=cos_y;
    cosines[2]=cos_z;
    origin=P;
	funct= new functions();
    n = 1;

}
ray::ray(float x1, float y1, float z1,float x2, float y2, float z2){
    float mag=funct->magnitude(x1-x2,y1-y2,z1-z2);
    cosines[0]=(x2-x1)/mag;
    cosines[1]=(y2-y1)/mag;
    cosines[2]=(z2-z1)/mag;
    origin->x=x1;
    origin->y=y1;
    origin->z=z1;
    n = 1;

}

void ray::initializeRay(Point &P1, Point &P2){

    float mag=funct->magnitude(P1.x-P2.x,P1.y-P2.y,P1.z-P2.z);
    cosines[0]=(P1.x-P2.x)/mag;
    cosines[1]=(P1.y-P2.y)/mag;
    cosines[2]=(P1.z-P2.z)/mag;
    origin->x=P1.x;
    origin->y=P1.y;
    origin->z=P1.z;
    n = 1;
}

void ray::initializeRay2(ray & r){    
    cosines[0] = r.cosines[0];
    cosines[1] = r.cosines[1];
    cosines[2] = r.cosines[2];
    origin[0] = r.origin[0];
    origin[1] = r.origin[1];
    origin[2] = r.origin[2];
}

