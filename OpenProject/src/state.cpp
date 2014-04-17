#include "state.h"

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