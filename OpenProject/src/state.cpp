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
float* state::get_foot_location(){
	return foot_location;
}
float* state::get_curr_velocity(){
	return curr_velocity;
}
float* state::get_target_velocity(){
	return target_velocity;
}

bool state::is_in_swing(int leg){
	if(gait_phase < swing_end[leg] && gait_phase > swing_start[leg]){

		return true ;
	}
	else 
		return false ;
}


void state::set_swing_start(int start_time, int leg){
	swing_start[leg] = start_time;
}

void state::set_swing_end(int start_time, int leg){
	swing_end[leg] = start_time;
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

void state::set_foot_location(float pos[3]){
	foot_location[0] = pos[0];
	foot_location[1] = pos[1];
	foot_location[2] = pos[2];
}