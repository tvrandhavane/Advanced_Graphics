#include <iostream>
#include <vector>
#include <math.h>


#include "raytracer.h"
#include "Sphere.h"
#include "rayIntersection.h"


using namespace std;


int main()
{
	raytracer *s ;
	s= new raytracer();
	//s->rayTraceCall(3);
	s->rayTraceCallAntiAliasing(1);
	s->createppm();


	return 0; 
}
