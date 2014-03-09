#include "raytracer.h"
#include <cmath>
#include <sstream>

#define maxcols 800
#define maxrows 600	

raytracer::raytracer(char* file_name){
 	funct = new functions();
	eye_wcs = new Point(0,0,0);
	frameCenter = new Point(0,0,0);

	frameView.resize(maxcols);
	for(int i = 0; i < frameView.size(); i++){
		frameView[i].resize(maxrows);
	}
	

	M.resize(3);
	for(int i = 0; i < 3; i++){
		M[i].resize(3);
	}

	backgroundColor= new color (0,0,0);
	
	ReflectedOrRefracted = new rayIntersection();
	init(file_name);

}

void raytracer::antiAliasing (int & temp_i , int & temp_j, int level ,color & pixelColor ){
	float deltau = (Wr - Wl)/maxcols;
	float deltav = (Wt - Wb)/maxrows;	
	srand (time(NULL));
	for(int k =0; k < 4; k++){

		
		float newi = rand() % 16;
		newi = newi/16 + temp_i;

		float newj = rand() % 16;
		newj = newj/16 + temp_j;
		ray r;
		
		vector<float> P1(3, 0);
		P1[0] = Wl + newi*deltau;
		P1[1] = Wt - newj*deltav;
		P1[2] = 0;
		
		vector<float> pointOnScreen = funct->vectorMatMult(P1, M);	
		pointOnScreen[0] +=  frameCenter->x;
		pointOnScreen[1] +=  frameCenter->y;
		pointOnScreen[2] +=  frameCenter->z;
		
		Point newp;
		newp.x = pointOnScreen[0];
		newp.y = pointOnScreen[1];
		newp.z = pointOnScreen[2];

		r.initializeRay(newp,*eye_wcs);
		color pixelCol;
		
		raytrace(level,r,pixelCol);
		//cout << "pixelCol " << newi << " "<< temp_i <<endl;

		pixelColor.R+=pixelCol.R;
		pixelColor.G+=pixelCol.G;
		pixelColor.B+=pixelCol.B;

	}
	pixelColor.R = pixelColor.R/4;
	pixelColor.G = pixelColor.G/4;
	pixelColor.B = pixelColor.B/4;
}

void raytracer::createppm(){
	clock_t t;
	t = clock();
	cout << t << endl;
	//cout << "lightcolor final = " <<  light_list[1]->location->x << " " << light_list[1]->location->y << " " << light_list[1]->location->z << " " << endl;
	string fileName = "./images/"; 
	stringstream ss;
	ss << t;
	string str = ss.str();
	fileName += str;
	fileName += ".ppm";
	ofstream ofs(fileName.c_str(), ios::out);
    ofs << "P3\n" << maxcols << " " << maxrows << "\n255\n";
    for(int j = 0; j < maxrows; j++){
    	for(int i = 0; i < maxcols; i++){
			ofs<<int(min(255, frameView[i][j]->R))<<" ";
            ofs<<int(min(255, frameView[i][j]->G))<<" ";
            ofs<<int(min(255, frameView[i][j]->B))<<" ";
        }
        ofs<<endl;
    }
    ofs.close();

}

bool raytracer::isIntersected(ray & r, ray & surfaceNormal, ray & reflected, ray & refracted, Point & intersectedPoint , color & pixelColor, int &object_type, material_property & property, bool & isRefracted){
	float min_t = 10000000;
	bool isIntersect = false ;
	float n;
	float t=-1.0;
	int index_of_min = 0;
	char ob_type;
	float checkDir = 0;
	for(int i = 0; i < Sphere_list.size(); i++){
		t = Sphere_list[i]->getValueOft(r);

		if((t > 0.0001) && (t <= min_t)){
			index_of_min = i;
			ob_type = 'S';
			min_t = t;
		}
	}

	for(int i = 0; i < Triangle_list.size(); i++){
		t = Triangle_list[i]->getValueOft(r);
		if((t > 0.0001) && (t <= min_t)){
			index_of_min = i;
			ob_type = 'T';
			min_t = t;
		}
	}

	if ((min_t > 0.0001) && (min_t < 10000000)){
		intersectedPoint.x = min_t * r.cosines[0] + (r. origin)->x ;
		intersectedPoint.y = min_t * r.cosines[1] + (r. origin)->y ;
		intersectedPoint.z = min_t * r.cosines[2] + (r. origin)->z ;

		if(ob_type == 'S'){
			Sphere_list[index_of_min]->getNormalAt(intersectedPoint, surfaceNormal, 1);
			checkDir = funct->dotproduct(surfaceNormal.cosines, r.cosines);
			if(checkDir > 0.0001){
				Sphere_list[index_of_min]->getNormalAt(intersectedPoint, surfaceNormal, 0);
			}
			//cout << "in sphere  " << endl;
			pixelColor.R = Sphere_list[index_of_min]->getColor()->R;
			pixelColor.G = Sphere_list[index_of_min]->getColor()->G;
			pixelColor.B = Sphere_list[index_of_min]->getColor()->B;
			object_type = Sphere_list[index_of_min]->getType();
			property.kd = Sphere_list[index_of_min]->property.kd;
			property.ks = Sphere_list[index_of_min]->property.ks;
			property.kr = Sphere_list[index_of_min]->property.kr;
			property.kt = Sphere_list[index_of_min]->property.kt;
			property.n = Sphere_list[index_of_min]->property.n;
			//cout << "property = " << property.n << endl;
		}
		
		if(ob_type == 'T'){
			Triangle_list[index_of_min]->getNormalAt(intersectedPoint, surfaceNormal, 1);
			checkDir = funct->dotproduct(surfaceNormal.cosines, r.cosines);
			if(checkDir > 0.0001){
				Triangle_list[index_of_min]->getNormalAt(intersectedPoint, surfaceNormal, 0);
			}
			pixelColor.R = Triangle_list[index_of_min]->getColor()->R;
			pixelColor.G = Triangle_list[index_of_min]->getColor()->G;
			pixelColor.B = Triangle_list[index_of_min]->getColor()->B;
			object_type = Triangle_list[index_of_min]->getType();
			property.kd = Triangle_list[index_of_min]->property.kd;
			property.ks = Triangle_list[index_of_min]->property.ks;
			property.kr = Triangle_list[index_of_min]->property.kr;
			property.kt = Triangle_list[index_of_min]->property.kt;	
			property.n = Triangle_list[index_of_min]->property.n;
		}
		if(object_type == 1){
			//initialize refracted ray
			ray newray1;
			if(checkDir < -0.0001){
				//air to object
				newray1 = ReflectedOrRefracted->getRefractedRay(intersectedPoint, r.n, property.n, r, surfaceNormal);
				newray1.n = property.n;
			}
			else{
				//object to air
				newray1 = ReflectedOrRefracted->getRefractedRay(intersectedPoint, property.n, r.n, r, surfaceNormal);
				newray1.n = 1;
			}
			
			if(funct->dotproduct(newray1.cosines, surfaceNormal.cosines) < -0.0001){
				//cout << "newray cosines = " <<  newray1.cosines[0] << " " <<  newray1.cosines[1] << " " <<  newray1.cosines[2] << " " << endl;
				Point newp1; //this new point is behind the intersected point
				newp1.x = intersectedPoint.x - newray1.cosines[0];
				newp1.y = intersectedPoint.y - newray1.cosines[1];
				newp1.z = intersectedPoint.z - newray1.cosines[2];
				refracted.initializeRay(intersectedPoint, newp1);
				isRefracted = true;
			}
			
			
			//initialize reflected ray
			ray newray2 = ReflectedOrRefracted->getReflectedRay(intersectedPoint, r, surfaceNormal);
			Point newp2; //this new point is behind the intersected point
			newp2.x = intersectedPoint.x - newray2.cosines[0];
			newp2.y = intersectedPoint.y - newray2.cosines[1];
			newp2.z = intersectedPoint.z - newray2.cosines[2];
			reflected.initializeRay(intersectedPoint, newp2);
		}


		
		isIntersect = true;
		t_count++;

	}

	
	return isIntersect;

}

bool raytracer::isBlocked(ray &r,  Point &p){
	float magnitude = funct->magnitude(r.origin->x - p.x, r.origin->y - p.y, r.origin->z - p.z);
	float t = 0;
	for(int i = 0; i < Sphere_list.size(); i++){
		t = Sphere_list[i]->getValueOft(r);
		if (t > 0.001){
			Point p1;
			p1.x = t * r.cosines[0] + (r. origin)->x;
			p1.y = t * r.cosines[1] + (r. origin)->y;
			p1.z = t * r.cosines[2] + (r. origin)->z;
			float temp_magnitude = funct->magnitude((r. origin)->x - p1.x, (r. origin)->y - p1.y, (r. origin)->z - p1.z);
			if(temp_magnitude < magnitude){
				return true;
			}
		}
	}
	for(int i = 0; i < Triangle_list.size(); i++){
		t = Triangle_list[i]->getValueOft(r);
		if (t > 0.0001){
			Point p1;
			p1.x = t * r.cosines[0] + (r. origin)->x;
			p1.y = t * r.cosines[1] + (r. origin)->y;
			p1.z = t * r.cosines[2] + (r. origin)->z;
			float temp_magnitude = funct->magnitude((r. origin)->x - p1.x, (r. origin)->y - p1.y, (r. origin)->z - p1.z);
			if(temp_magnitude < magnitude){
				return true;
			}
		}
	}

	
	return false;
}

vector<float> raytracer::getLightColor(Point &  intersectedPoint, ray & surfaceNormal){
	int size = light_list.size();
	vector<float> c(3, 0);
	for (int i = 0;i < size ; i++){
		ray r;
		Point P;
		P.x = light_list[i]->location->x;
		P.y = light_list[i]->location->y;
		P.z = light_list[i]->location->z;
		r.initializeRay(P, intersectedPoint);
		r.origin->x = intersectedPoint.x;
		r.origin->y = intersectedPoint.y;
		r.origin->z = intersectedPoint.z;
		//cout << "lightcolor light color = " <<  light_list[1]->location->x << " " << light_list[1]->location->y << " " << light_list[1]->location->z << " " << endl;
		if(funct->dotproduct(r.cosines, surfaceNormal.cosines) > 0.0001){
			//cout << "isBlocked " << endl;
			//cout << "in dotproduct = " << endl;
			if(!isBlocked(r, P)){
				//cout << "isBlocked " << endl;
				float cosine = funct->dotproduct(r.cosines, surfaceNormal.cosines);
				int distance = 0;
				distance = funct->magnitude(light_list[i]->location->x - intersectedPoint.x,
											light_list[i]->location->y - intersectedPoint.y,
											light_list[i]->location->z - intersectedPoint.z);
				//cout << "distance = " <<  distance << endl;
				if(cosine > 0.0001){
					c[0] += (cosine*(light_list[i])->lightcolor->R)/(distance*distance);
					c[1] += (cosine*(light_list[i])->lightcolor->G)/(distance*distance);
					c[2] += (cosine*(light_list[i])->lightcolor->B)/(distance*distance);
				}
			}
		}
	}

	return c;
}

vector<float> raytracer::getLightColorSpeculer(ray & r, Point &  intersectedPoint, ray & surfaceNormal){
	int size = light_list.size();
	vector<float> c(3, 0);
	for (int i = 0;i < size ; i++){
		ray lightray;
		ray lightrayOpp;
		Point P;
		//cout << "lightcolor light color init = " <<  light_list[1]->location->x << " " << light_list[1]->location->y << " " << light_list[1]->location->z << " " << endl;
		P.x = light_list[i]->location->x;
		P.y = light_list[i]->location->y;
		P.z = light_list[i]->location->z;
		//cout << "lightcolor light color = " <<  light_list[1]->location->x << " " << light_list[1]->location->y << " " << light_list[1]->location->z << " " << endl;
		
		lightray.initializeRay(P, intersectedPoint);
		lightray.origin->x = intersectedPoint.x;
		lightray.origin->y = intersectedPoint.y;
		lightray.origin->z = intersectedPoint.z;
		lightrayOpp.initializeRay(intersectedPoint, P);
		if(funct->dotproduct(lightray.cosines, surfaceNormal.cosines) > 0.0001){
			if(!isBlocked(lightray, intersectedPoint)){
				ray R = ReflectedOrRefracted->getReflectedRay(intersectedPoint, lightrayOpp, surfaceNormal);
				vector<float> V(3, 0);
				V[0] = -1 * r.cosines[0];
				V[1] = -1 * r.cosines[1]; 
				V[2] = -1 * r.cosines[2];		

				float cosine = funct->dotproduct(R.cosines, V);
				int distance = 0;
				distance = funct->magnitude(light_list[i]->location->x - intersectedPoint.x,
											light_list[i]->location->y - intersectedPoint.y,
											light_list[i]->location->z - intersectedPoint.z);
				if(cosine > 0.0001){
					cosine = pow(cosine, 21);
					c[0] += (cosine*(light_list[i])->lightcolor->R)/(distance*distance);
					c[1] += (cosine*(light_list[i])->lightcolor->G)/(distance*distance);
					c[2] += (cosine*(light_list[i])->lightcolor->B)/(distance*distance);
				}
			}
		}
			
	}

	return c;
}

void raytracer::rayTraceCallAntiAliasing(int level){
	t_count=0;		
	for(int i = 0; i <maxcols;i++){
		for(int j = 0; j <maxrows ;j++){
			color *pixelColor = new color();
			antiAliasing(i,j,level,*pixelColor);
			frameView[i][j]=pixelColor;

		}
	}
}

void raytracer::rayTraceCall(int level){
	t_count=0;
	float deltau = (Wr - Wl)/maxcols;
	float deltav = (Wt - Wb)/maxrows;
	for(int i = 0; i < maxcols; i++){
		for(int j = 0; j < maxrows; j++){
			//cout << "i = "<< i << " " << j << endl;
			
			ray r;
			vector<float> P1(3, 0);
			P1[0] = Wl + i*deltau;
			P1[1] = Wt - j*deltav;
			P1[2] = 0;
			
			vector<float> pointOnScreen = funct->vectorMatMult(P1, M);	
			pointOnScreen[0] +=  frameCenter->x;
			pointOnScreen[1] +=  frameCenter->y;
			pointOnScreen[2] +=  frameCenter->z;
			
			Point newp;
			newp.x = pointOnScreen[0];
			newp.y = pointOnScreen[1];
			newp.z = pointOnScreen[2];
			//cout << "eye = " << eye_wcs->x << " " << eye_wcs->y << " " << eye_wcs->z  << endl;
			//cout << "newp = " << newp.x << " " << newp.y << " " << newp.z << endl;
			
			r.initializeRay(newp, *eye_wcs);

			color * pixelColor = new color();
			r.n = 1;

			raytrace(level,r,*pixelColor);		

			frameView[i][j] = pixelColor;   
			//cout << "pixelColor = "<< pixelColor->R << " " << pixelColor->G << " " << pixelColor->B << endl;
			
			
			
		}
	}
}

void raytracer::raytrace(int level ,ray & r ,color & pixelColor){
	//cout << "level = " << level << endl;
	if(level > 0){
		ray *reflected = new ray();
		ray *refracted = new ray();
		ray *normal = new ray();
		Point *pointOfIntersection = new Point();
		int object_type;
		material_property obj_prop;
		bool isRefracted = false;
		if(isIntersected(r, *normal, *reflected, *refracted, *pointOfIntersection, pixelColor, object_type, obj_prop, isRefracted)){
			//cout << "intersected " << endl;
			color* reflectedCol = new color();
			color* refractedCol = new color();
			float MatFact=.5;

			if(object_type == 0){
				//cout << "diffuse " << endl;
				vector<float> lightFactor = getLightColor(*pointOfIntersection, *normal);
				pixelColor.R = (int) (pixelColor.R * obj_prop.kd * lightFactor[0]);
				pixelColor.G = (int) (pixelColor.G * obj_prop.kd * lightFactor[1]);
				pixelColor.B = (int) (pixelColor.B * obj_prop.kd * lightFactor[2]);
				/*if((pixelColor.R == 0)&&(pixelColor.B == 0)&&(pixelColor.G == 0)){
					cout << "lightFactorR = " << lightFactor[0] << endl;
				}*/
				if(pixelColor.R > 255){
					pixelColor.R = 255;
				}
				if(pixelColor.G > 255){
					pixelColor.G = 255;
				}
				if(pixelColor.B > 255){
					pixelColor.B = 255;
				}
 			}
			else if(object_type == 1){
				//cout << "specular " << endl;
				if(level > 0){

					raytrace(level-1, *reflected, *reflectedCol);
					if(isRefracted){
						//cout << "is refracted " << endl;
						raytrace(level-1, *refracted, *refractedCol);
					}		
				}
				else{ 
					reflectedCol->R = 0;
					reflectedCol->G = 0;
					reflectedCol->B = 0;
					refractedCol->R = 0;
					refractedCol->G = 0;
					refractedCol->B = 0;
				}
				

				vector<float> lightFactorDiff = getLightColor(*pointOfIntersection, *normal);
				vector<float> lightFactorSpeculer = getLightColorSpeculer(r,*pointOfIntersection,*normal);

				pixelColor.R = (int)(obj_prop.kr*reflectedCol->R) +
								(int)(obj_prop.kt*refractedCol->R) +
								(int) (pixelColor.R * obj_prop.kd * lightFactorDiff[0]) +
								(int) (pixelColor.R * obj_prop.ks * lightFactorSpeculer[0]);
				pixelColor.G = (int)(obj_prop.kr*reflectedCol->G) +
								(int)(obj_prop.kt*refractedCol->G) +
								(int) (pixelColor.G * obj_prop.kd * lightFactorDiff[1]) +
								(int) (pixelColor.G * obj_prop.ks * lightFactorSpeculer[1]);
				pixelColor.B = (int)(obj_prop.kr*reflectedCol->B) +
								(int)(obj_prop.kt*refractedCol->B) +
								(int) (pixelColor.B * obj_prop.kd * lightFactorDiff[2]) +
								(int) (pixelColor.B * obj_prop.ks * lightFactorSpeculer[2]);
				if(pixelColor.R > 255){
					pixelColor.R = 255;
				}
				if(pixelColor.G > 255){
					pixelColor.G = 255;
				}
				if(pixelColor.B > 255){
					pixelColor.B = 255;
				}
			}

		}
		else{
			//cout << "Not intersected " << endl;
			pixelColor.R = backgroundColor->R;
			pixelColor.G = backgroundColor->G;
			pixelColor.B = backgroundColor->B;		
			//cout << "Not intersected " << endl;
		}
	}

}

void raytracer::init(char *file_name){

	string spec_file , data;
	vector<string> list;
	Point eye;
	vector<float> vup(3, 0);
	vector<float> n(3, 0);
	
	cout << "Reading scene file: " << endl;
	ifstream specs (file_name);
	if (specs.is_open()){
		while(getline(specs,data)){
			list=funct->split(data,' ');	
			if(list[0].compare("S")==0){
				Sphere *s ;
				int rad=atoi(list[1].c_str());
				Point *center;
				color *col;
				int type;
				center= new Point(atof(list[2].c_str()),atof(list[3].c_str()),atof(list[4].c_str()));
				col = new color(atoi(list[5].c_str()),atoi(list[6].c_str()),atoi(list[7].c_str()));	
				type = atoi(list[8].c_str());
				material_property prop;
				prop.kd = atof(list[9].c_str());
				prop.ks = atof(list[10].c_str());
				prop.kr = atof(list[11].c_str());
				prop.kt = atof(list[12].c_str());
				prop.n = atof(list[13].c_str());
				//cout << "n = "<< prop.n << endl;
				s = new Sphere(center, rad,  col, type, prop);

				Sphere_list.push_back(s);
			}
			if(list[0].compare("L")==0){
				light * l ;
				Point *p;
				color *c;
				p = new Point(atof(list[1].c_str()),atof(list[2].c_str()),atof(list[3].c_str()));
				c = new color(atoi(list[4].c_str()),atoi(list[5].c_str()),atoi(list[6].c_str()));	
				l = new light(p, c);
				//cout << "light pos = " << p->x << " " << p->y << " " << p->z << endl;
				light_list.push_back(l);
			}
			if(list[0].compare("E")==0){
				eye.x = atof(list[1].c_str());
				eye.y = atof(list[2].c_str());
				eye.z = atof(list[3].c_str()); 
			}
			if(list[0].compare("VUP")==0){
				vup[0] = atof(list[1].c_str());
				vup[1] = atof(list[2].c_str());
				vup[2] = atof(list[3].c_str()); 
			}
			if(list[0].compare("N")==0){
				n[0] = atof(list[1].c_str());
				n[1] = atof(list[2].c_str());
				n[2] = atof(list[3].c_str()); 
			}
			if(list[0].compare("R")==0){
				frameCenter->x = atof(list[1].c_str());
				frameCenter->y = atof(list[2].c_str());
				frameCenter->z = atof(list[3].c_str()); 
			}
			if(list[0].compare("W")==0){
				Wr = atof(list[1].c_str());
				Wl = atof(list[2].c_str());
				Wt = atof(list[3].c_str()); 
				Wb = atof(list[4].c_str()); 
			}
			if(list[0].compare("T")==0){
				Triangle *t ;
				Point p1, p2, p3;
				color *c;
				int type;
				p1.x = atof(list[1].c_str());
				p1.y = atof(list[2].c_str());
				p1.z = atof(list[3].c_str());

				p2.x = atof(list[4].c_str());
				p2.y = atof(list[5].c_str());
				p2.z = atof(list[6].c_str());

				p3.x = atof(list[7].c_str());
				p3.y = atof(list[8].c_str());
				p3.z = atof(list[9].c_str());

				c = new color(atoi(list[10].c_str()),atoi(list[11].c_str()),atoi(list[12].c_str()));

				type = atoi(list[13].c_str());

				material_property prop;
				prop.kd = atof(list[14].c_str());
				prop.ks = atof(list[15].c_str());
				prop.kr = atof(list[16].c_str());
				prop.kt = atof(list[17].c_str());
				prop.n = atof(list[18].c_str());


				t= new Triangle(p1, p2, p3, prop, c, type);
				Triangle_list.push_back(t);
			}

		}
		list.clear();
	}
	specs.close();

	//initialize M
	float dot = funct->dotproduct(vup, n);
	vector<float> v(3, 0);
	for(int i = 0; i < 3; i++){
		v[i] = vup[i] - dot*n[i];
	}
	dot = funct->magnitude(v[0], v[1], v[2]);
	for(int i = 0; i < 3; i++){
		v[i] /= dot;
	}

	vector<float> u = funct->crossproduct( n[0], n[1], n[2], v[0], v[1], v[2]);
	for(int i = 0; i < 3; i++){
		M[i][0] = u[i];
	}
	for(int i = 0; i < 3; i++){
		M[i][1] = v[i];
	}
	for(int i = 0; i < 3; i++){
		M[i][2] = n[i];
	}
	
	//initialize Eye_wcs
	vector<float> eye_vec(3, 0);
	eye_vec[0] = eye.x;
	eye_vec[1] = eye.y;
	eye_vec[2] = eye.z;
	v = funct->vectorMatMult(eye_vec, M);
	eye_wcs->x = v[0] + frameCenter->x;
	eye_wcs->y = v[1] + frameCenter->y;
	eye_wcs->z = v[2] + frameCenter->z;
}
