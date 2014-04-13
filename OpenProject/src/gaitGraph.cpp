#include "gaitGraph.h"

gaitGraph::gaitGraph(){
	gait_phase = 0;
}

float gaitGraph::get_gait_phase(){
	return ((float)gait_phase)/((float)stride_period);
}

int gaitGraph::get_stride_period(){
	return stride_period;
}

void gaitGraph::set_stride_period(int stride_period){
	this->stride_period = stride_period;
}

void gaitGraph::set_swing_start(int start_time, int leg){
	swing_start[leg] = start_time;
}

void gaitGraph::set_swing_end(int start_time, int leg){
	swing_start[leg] = start_time;
}