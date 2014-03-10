#include "Sphere.h"

Sphere::Sphere(){
	funct= new functions();
	mPre = new matrix(4, 4);
	mPost = new matrix(4, 4);
	mPre->makeIdentity();
	mPost->makeIdentity();
}

Sphere::Sphere(Point *Center ,float rad, color* s_col, int type, material_property &p){
	center = Center;
	radius = rad;
	s_color = s_col;
	this->type = type;
	property.kd = p.kd;
	property.kr = p.kr;
	property.ks = p.ks;
	property.kt = p.kt;
	property.n = p.n;
	mPre = new matrix(4, 4);
	mPost = new matrix(4, 4);
	mPre->makeIdentity();
	mPost->makeIdentity();
}

float Sphere::getRadius(){
	return radius;
}

color* Sphere::getColor(){
	return s_color;
}

Point* Sphere::getCenter(){
	return center;
}

int Sphere::getType(){
	return type;
}


float Sphere::getValueOft(ray &r){
	float a = 1;
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
	return -1;
}

void Sphere::getNormalAt(Point &P, ray & r, int dir){
	if(dir == 1){
		r.initializeRay(P, *center);
	}
	else{
		r.initializeRay(*center, P);
	}
}

void Sphere::preMultMatrix(matrix *mat){
	mPre->preMultMatrix(mat);
}

void Sphere::postMultMatrix(matrix *mat){
	mPost->postMultMatrix(mat);
}
