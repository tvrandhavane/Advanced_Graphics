#include "state.h"
#include <math.h>  

state::state(){
	gait_phase =0;
	number_joints = 10 ;
}
int state::get_stride_period(){
	return stride_period;
}
int state::get_number_joints(){
	return number_joints;
}
int state::get_gait_phase(){
	return gait_phase;
}
void state::set_gait_phase(int phase){
	gait_phase = phase;
}


Matrix & state::transferFrame(Matrix & M ,float length, float angleX, float angleY, float angleZ){
        Matrix *A = new Matrix(4,4);
        Matrix rotaion = Matrix(4,4);
        Matrix translate = Matrix(4,4);
        rotaion(1,1) = cos(angleZ);
        rotaion(1,2) = -1*sin(angleZ);
 		rotaion(2,1) = sin(angleZ);   
 		rotaion(2,2) = cos(angleZ);
 		rotaion(3,3) = 1;
 		rotaion(4,4) = 1;
 		translate(1,1) = 1;
 		translate(2,2) = 1;
 		translate(3,3) = 1;
 		translate(4,4) = 1;
 		translate(1,4) = -1*length ;
 		*A = function.Multiply(rotaion,translate);  
 		*A = function.Multiply(M,*A);  
 		return *A ;            
}