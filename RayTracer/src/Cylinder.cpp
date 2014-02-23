#include "Cylinder.h"

Cylinder::Cylinder(){
	funct= new functions();
}

Cylinder::Cylinder(Point *BaseCenter, Point *TopCenter, float  rad, color* s_col, int type, material_property &p){
	topCenter = TopCenter;
	baseCenter = BaseCenter;
	radius = rad;
	s_color = s_col;
	this->type = type;	
	property.kd = p.kd;
	property.kr = p.kr;
	property.ks = p.ks;
	property.kt = p.kt;
	property.n = p.n;
}

float Cylinder::getRadius(){
	return radius;
}

color* Cylinder::getColor(){
	return s_color;
}

Point* Cylinder::getBaseCenter(){
	return baseCenter;
}

Point* Cylinder::getTopCenter(){
	return topCenter;
}

int Cylinder::getType(){
	return type;
}


float Cylinder::getValueOft(ray &r){
	/*float a = 1;
	float b = - 2*(r.cosines[0]*((center->x) -(r.origin->x))+r.cosines[1]*((center->y)-r.origin->y)+r.cosines[2]*(center->z-r.origin->z));
	float c = funct->square(center->x-r.origin->x)+funct->square(center->y-r.origin->y)+funct->square(center->z-r.origin->z)-funct->square(radius);

	float D = funct->square(b) - 4 * a * c;
	float t1 = -1;
	float t2 = -1;
	if(D >= 0.0001){
		t1=(-1*b+sqrt(D))/(2*a);
		t2=(-1*b-sqrt(D))/(2*a);
		if(t1 < t2){
			if((t1 <= 0.0001) && (t1 >= 0.0001)){
				return t2;
			}
			return t1;
		}
		else{
			if((t2 <= 0.0001) && (t2 >= 0.0001)){
				return t1;
			}
			return t2;
		}
	}
	return -1;*/
		
}

void Cylinder::getNormalAt(Point &P, ray & r, int dir){
	/*Point newp;
	newp.x = P.x - baseCenter.x;
	newp.y = P.y - baseCenter.y;
	newp.z = P.z - baseCenter.z;

	Point newp2;
	newp2.x = 0;
	newp2.y = newp.y;
	newp2.z = 0;

	if(dir == 1){
		r.initializeRay(newp, newp2);
		r.origin->x += baseCenter.x;
		r.origin->y += baseCenter.y;
		r.origin->z += baseCenter.z;
	}
	else{
		r.initializeRay(newp2, newp);
		r.origin->x = P.x;
		r.origin->y = P.y;
		r.origin->z = P.z;

	}
	*/
}

