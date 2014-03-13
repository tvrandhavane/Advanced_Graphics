#include <iostream>
#include <vector>
#include <math.h>


#include "raytracer.h"
#include "Sphere.h"
#include "rayIntersection.h"
#include "matrix.h"

using namespace std;


int main(int argc, char*argv[])
{
	raytracer *s ;
	s= new raytracer(argv[1]);
	s->rayTraceCall(3);
	//s->rayTraceCallAntiAliasing(1);
	s->createppm();

	/*matrix * m = new matrix(4, 4);
	matrix* mat = new matrix(4,4);
	mat->set(0,0,1);
	mat->set(1,1,1);
	mat->set(2,2,1);
	mat->set(3,3,1);
	mat->set(0,3,4);
	mat->set(1,3,5);
	mat->set(2,3,6);

	m->set(0,0,1);
	m->set(1,1,1);
	m->set(2,2,1);
	m->set(3,3,1);
	m->print();
	cout << endl;
	mat->print();
	cout << endl;
	m->postMultMatrix(mat);
	m->print();
	/*mat->postMultMatrix(mat);
	cout << endl;
	mat->print();*/
	/*cout << endl;

	vector<float> v(4, 0);
	v[0] = -3;
	v[1] = -5;
	v[2] = -6;
	v[3] = 1;

	for(int i = 0; i < v.size(); i++){
		cout << v[i] << " ";
	}
	cout << endl;
	cout << endl;
	vector<float> v1 = m->postMultVector(v);
	for(int i = 0; i < v1.size(); i++){
		cout << v1[i] << " ";
	}
	cout << endl;*/

	return 0;
}
