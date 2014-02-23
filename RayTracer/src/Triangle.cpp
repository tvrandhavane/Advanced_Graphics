#include "Triangle.h"

Triangle::Triangle()
{
    funct = new functions();
}

Triangle::Triangle(Point &temp_V1, Point &temp_V2, Point &temp_V3, material_property & pr, color * temp_c, int temp_type){

    V1.x = temp_V1.x;
    V1.y = temp_V1.y;
    V1.z = temp_V1.z;

    V2.x = temp_V2.x;
    V2.y = temp_V2.y;
    V2.z = temp_V2.z;

    V3.x = temp_V3.x;
    V3.y = temp_V3.y;
    V3.z = temp_V3.z;

    property.kd = pr.kd;
    property.ks = pr.ks;
    property.kr = pr.kr;
    property.kt = pr.kt;
    property.n = pr.n;
    Normal.resize(3);
    vector<float> n = funct->crossproduct(V1.x-V2.x,
                                    V1.y-V2.y,
                                    V1.z-V2.z,
                                    V1.x-V3.x,
                                    V1.y-V3.y,
                                    V1.z-V3.z);
    float mag = funct->magnitude(n[0],n[1],n[2]);
    for(int i=0 ;i < 3; i++){
        Normal[i]=n[i]/mag;
    }

    c = temp_c;

    type = temp_type;
}

color* Triangle::getColor(){
    return c;
}

int Triangle::getType(){
    return type;
}

void Triangle::getNormalAt(Point &P, ray & r, int dir){
    r.origin->x = P.x;
    r.origin->y = P.y;
    r.origin->z = P.z;

    if(dir == 1){
        r.cosines[0] = Normal[0];
        r.cosines[1] = Normal[1];
        r.cosines[2] = Normal[2];
    }
    else{
        r.cosines[0] = -1*Normal[0];
        r.cosines[1] = -1*Normal[1];
        r.cosines[2] = -1*Normal[2];
    }

}

float Triangle::getValueOft(ray &r){
    float constant_d = Normal[0]*V1.x + Normal[1]*V1.y + Normal[2]*V1.z;
    float dot_ray_Normal = funct->dotproduct(r.cosines,Normal);

    if((dot_ray_Normal <= 0.0001) && (dot_ray_Normal >= -0.0001)){
        return -1;
    }
    else{
        vector<float> org ;
        org.resize(3);
        org[0]=r.origin->x;
        org[1]=r.origin->y;
        org[2]=r.origin->z;
        float dot_Rorigin_Normal = funct->dotproduct(org,Normal);
        float t = -1*(-1*constant_d + dot_Rorigin_Normal)/dot_ray_Normal;
        if(t <= 0.0001){
            return -1;
        }    
        else{
            Point intP;
            intP.x = r.origin->x + r.cosines[0]*t;
            intP.y = r.origin->y + r.cosines[1]*t;
            intP.z = r.origin->z + r.cosines[2]*t;

            vector <float> n1 = funct->crossproduct(V1.x-V2.x,
                                        V1.y-V2.y,
                                        V1.z-V2.z,
                                        intP.x-V2.x,
                                        intP.y-V2.y,
                                        intP.z-V2.z);
            vector <float> n2 = funct->crossproduct(V2.x-V3.x,
                                        V2.y-V3.y,
                                        V2.z-V3.z,
                                        intP.x-V3.x,
                                        intP.y-V3.y,
                                        intP.z-V3.z);
            vector <float> n3 = funct->crossproduct(V3.x-V1.x,
                                        V3.y-V1.y,
                                        V3.z-V1.z,
                                        intP.x-V1.x,
                                        intP.y-V1.y,
                                        intP.z-V1.z);
            float dot_1= funct->dotproduct(Normal,n1);
            float dot_2= funct->dotproduct(Normal,n2);
            float dot_3= funct->dotproduct(Normal,n3);
            if((dot_1 > -0.0001) && (dot_2 > -0.0001) && (dot_3 > -0.0001)){
                return t;
            }
            else if((dot_1 < 0.0001) && (dot_2 < 0.0001) && (dot_3 < 0.0001)){
                return t;
            }
            else{
                return -1;
            }
        }
    }
}

