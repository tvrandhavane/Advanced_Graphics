#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "Sphere.h"
#include "rayIntersection.h"
#include "Triangle.h"
#include "Point.h"
#include "light.h"


using namespace std;

class raytracer{

	private:
		vector <Sphere *> Sphere_list;
		vector <Triangle *> Traingle_list;
		vector <Triangle *> Triangle_list;
		vector <light *> light_list;
		functions *funct;
		Point * eye_wcs;
		Point * frameCenter;
		float Wr, Wl, Wb, Wt;
		vector<vector<float> > M;
		int trace_level;
		color * backgroundColor;
		rayIntersection * ReflectedOrRefracted;
		vector<vector<color*> > frameView;
		bool isIntersected(ray & r, ray & surfaceNormal, ray & reflected, ray & refracted, Point & intersectedPoint , color & pixelColor, int &object_type, material_property & property, bool & isRefracted);
		vector<float> getLightColor(Point &  intersectedPoint, ray & surfaceNormal);
		vector<float> getLightColorSpeculer(ray & r, Point &  intersectedPoint, ray & surfaceNormal);

	public:
		int t_count;
		void init();
		raytracer();
		void createppm();
		int getFrameHeight();
		void setFrameHeight(int height);
		int getFrameWidth();
		void setFrameWidth(int width);
		Point * getEyePosition();
		void setEyePosition(Point * P);
		Point * getFramePosition();
		void setFramePosition(Point * P);
		void rayTraceCall(int level);
		void raytrace(int level ,ray & r ,color & pixelColor);
		bool isBlocked(ray &r,  Point &p);
		void antiAliasing (int & temp_i , int & temp_j, int level ,color & pixelColo);
		void rayTraceCallAntiAliasing(int level);




};




#endif
