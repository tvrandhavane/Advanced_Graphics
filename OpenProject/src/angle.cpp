#include "angle.h"

angle::angle(int upper_limit, int lower_limit){
	this->upper_limit = upper_limit;
	this->lower_limit = lower_limit;
}
angle::angle(){
	
}
int angle::get_value(){
	return current_value;
}

int angle::set_value(int value){
	if(value > upper_limit){
		current_value = upper_limit;
		return 1;
	}
	else if(value < lower_limit){
		current_value = lower_limit;
		return -1;
	}
	else{
		current_value = value;
		return 0;
	}
}

void angle::set_upper_limit(int value){
	upper_limit = value;
}

void angle::set_lower_limit(int value){
	lower_limit = value;
}