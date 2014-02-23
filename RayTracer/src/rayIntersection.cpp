#include "rayIntersection.h"

rayIntersection::rayIntersection(){

	funct = new functions();
}

ray & rayIntersection::getReflectedRay(Point &P, ray & r, ray & normal){
	
	ray * r1;
	Point *Pt;
	float cos_x;
	float cos_y;
	float cos_z;

	float dot;
	dot = funct->dotproduct(r.cosines, normal.cosines);
	
	cos_x=r.cosines[0]-2*dot*normal.cosines[0];
	cos_y=r.cosines[1]-2*dot*normal.cosines[1];
	cos_z=r.cosines[2]-2*dot*normal.cosines[2];
	Pt= new Point(P.x,P.y,P.z);
	r1 = new ray(cos_x,cos_y,cos_z,Pt);
	return *r1;

	}

ray & rayIntersection::getRefractedRay(Point &P, float n1 , float n2 ,ray & incident_r, ray & normal){

	ray * r1;
	Point *Pt;
	Pt = new Point (P.x , P.y ,P.z);
	float cos_x;
	float cos_y;
	float cos_z;
	float dot = funct->dotproduct(incident_r.cosines,normal.cosines);
	float cos_theta = sqrt(1 - funct->square(n1/n2) * (1-funct->square(dot))) ;
	
	
	float fact = cos_theta+(n1/n2)*dot;

	
	cos_x = (n1/n2)*(incident_r.cosines[0])-fact*normal.cosines[0];
	cos_y = (n1/n2)*(incident_r.cosines[1])-fact*normal.cosines[1];
	cos_z = (n1/n2)*(incident_r.cosines[2])-fact*normal.cosines[2];
	r1 = new ray(cos_x,cos_y,cos_z, Pt);
	return *r1;
}


