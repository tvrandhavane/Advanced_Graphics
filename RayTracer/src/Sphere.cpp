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
	vector<float> pointa(4, 1);
	vector<float> pointb(4, 1);

	pointa[0] = r.origin->x;
	pointa[1] = r.origin->y;
	pointa[2] = r.origin->z;

	pointb[0] = r.origin->x + r.cosines[0];
	pointb[1] = r.origin->y + r.cosines[1];
	pointb[2] = r.origin->z + r.cosines[2];

	vector<float> pointaTransformed = mPre->postMultVector(pointa);
	vector<float> pointbTransformed = mPre->postMultVector(pointb);

	vector<float> cosines(3, 0);
	cosines[0] = pointbTransformed[0] - pointaTransformed[0];
	cosines[1] = pointbTransformed[1] - pointaTransformed[1];
	cosines[2] = pointbTransformed[2] - pointaTransformed[2];

	float cosineMagnitude = cosines[0]*cosines[0] + cosines[1]*cosines[1] + cosines[2]*cosines[2];

	cosines[0] /= cosineMagnitude;
	cosines[1] /= cosineMagnitude;
	cosines[2] /= cosineMagnitude;

	float a = 1;
	float b = -2*(cosines[0]*((center->x) -pointaTransformed[0])+cosines[1]*((center->y)-pointaTransformed[1])+cosines[2]*(center->z-pointaTransformed[2]));
	float c = funct->square(center->x-pointaTransformed[0])+funct->square(center->y-pointaTransformed[1])+funct->square(center->z-pointaTransformed[2])-funct->square(radius);

	float D = funct->square(b) - 4 * a * c;
	float t1 = -1;
	float t2 = -1;
	float t = -1;
	if(D >= 0.0001){
		t1=(-1*b+sqrt(D))/(2*a);
		t2=(-1*b-sqrt(D))/(2*a);
		if(t1 < t2){
			if((t1 <= 0.0001) && (t1 >= 0.0001)){
				t = t2;
			}
			else{
				t = t1;
			}
		}
		else{
			if((t2 <= 0.0001) && (t2 >= 0.0001)){
				t = t1;
			}
			else{
				t = t2;
			}
		}
	}

	vector<float> intersectionPoint(4, 0);
	intersectionPoint[0] = pointaTransformed[0] + t*cosines[0];
	intersectionPoint[1] = pointaTransformed[1] + t*cosines[1];
	intersectionPoint[2] = pointaTransformed[2] + t*cosines[2];

	vector<float> intersectionPointTransformed = mPost->postMultVector(intersectionPoint);
	float returnT = (intersectionPointTransformed[0] - pointa[0])/r.cosines[0];

	return returnT;
}

void Sphere::getNormalAt(Point &P, ray & r, int dir){
	vector<float> intersectionPoint(4, 1);

	intersectionPoint[0] = P.x;
	intersectionPoint[1] = P.y;
	intersectionPoint[2] = P.z;

	vector<float> intersectionPointTransformed = mPre->postMultVector(intersectionPoint);

	Point pTransformed(intersectionPointTransformed[0], intersectionPointTransformed[1], intersectionPointTransformed[2]);

	ray newray;
	if(dir == 1){
		newray.initializeRay(P, *center);
	}
	else{
		newray.initializeRay(*center, P);
	}

	vector<float> cosines(4, 1);

	cosines[0] = newray.cosines[0];
	cosines[1] = newray.cosines[1];
	cosines[2] = newray.cosines[2];

	vector<float> cosinesTransformed = (mPost->transform())->postMultVector(cosines);

	float cosineMagnitude = cosinesTransformed[0]*cosinesTransformed[0] +
					cosinesTransformed[1]*cosinesTransformed[1] +
					cosinesTransformed[2]*cosinesTransformed[2];

	cosinesTransformed[0] /= cosineMagnitude;
	cosinesTransformed[1] /= cosineMagnitude;
	cosinesTransformed[2] /= cosineMagnitude;

	r.origin = &P;
	r.cosines[0] = cosinesTransformed[0];
	r.cosines[1] = cosinesTransformed[1];
	r.cosines[2] = cosinesTransformed[2];
}

void Sphere::preMultMatrix(matrix *mat){
	mPre->preMultMatrix(mat);
}

void Sphere::postMultMatrix(matrix *mat){
	mPost->postMultMatrix(mat);
}

void Sphere::printPreMatrix(){
	mPre->print();
}

void Sphere::printPostMatrix(){
	mPost->print();
}