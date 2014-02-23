#include <iostream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;


class functions{

	public:
	float square(float x){
	    return x*x;
	}

	vector<float> vectorMatMult(vector<float> &point, vector<vector <float> > &matrix){
		vector<float> vec(point.size(), 0);
		for(int i = 0; i < point.size(); i++){
			vec[i] = dotproduct(point, matrix[i]);
		}
		return vec;
	}

	

	float dotproduct(vector<float> &cosine1, vector<float> &cosine2){
		float value = 0;
		for(int i = 0; i < cosine1.size(); i++){
			value += cosine1[i]*cosine2[i];
		}
		return value;
	}

	float magnitude(float x1, float y1, float z1){
	    return sqrt(x1*x1+y1*y1+z1*z1);
	}
	
	vector<float> crossproduct(float x1, float y1, float z1, float x2, float y2, float z2){
	    vector<float> vec;
	    vec.resize(3);
	    vec[0] = y1*z2-z1*y2;
	    vec[1] = -x1*z2+z1*x2;
	    vec[2] = x1*y2-y1*x2;

	    return vec;

	}
	
	vector<string> split(string input , char c){

		vector<string> list;
		string st="";
		int size= input.size();
		for(int i=0;i<size;i++){
			if(input[i]==c || i==size-1){
				if(i==size-1)
					st+=input[i];
				list.push_back(st);
				st="";
			}
			else{
				st+=input[i];
			}
		}
	
	 return list;

	}

};
