#include "state.h"

state::state(){
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