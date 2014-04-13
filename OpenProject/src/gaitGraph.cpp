#include "gaitGraph.h"

gaitGraph::gaitGraph(int stride_period){
	state_parameters->stride_period = stride_period;
}

int gaitGraph::get_stride_period(){
	return state_parameters->stride_period;
}


float gaitGraph::get_gait_phase(){
	return ((float)state_parameters->gait_phase)/((float)state_parameters->stride_period);
}


void gaitGraph::set_stride_period(int stride_period){
	state_parameters->stride_period = stride_period;
}

void gaitGraph::set_swing_start(int start_time, int leg){
	state_parameters->swing_start[leg] = start_time;
}

void gaitGraph::set_swing_end(int start_time, int leg){
	state_parameters->swing_start[leg] = start_time;
}