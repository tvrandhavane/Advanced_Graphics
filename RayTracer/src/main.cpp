#include <iostream>
#include <vector>
#include <math.h>


#include "raytracer.h"
#include "Sphere.h"
#include "rayIntersection.h"


using namespace std;


int main(int argc, char*argv[])
{
	raytracer *s ;
	s= new raytracer(argv[1]);
	s->rayTraceCall(3);
	//s->rayTraceCallAntiAliasing(1);
	s->createppm();


	return 0; 
}
