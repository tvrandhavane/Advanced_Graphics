#include <iostream>
#include "controller.h"
#include "state.h"

using namespace std;

state * state_parameters;

void compute_tau_pd(vector<float> &tau){

}

void compute_tau_vf(vector<float> &tau){

}

void add_vector(vector<float> &result, vector<float> &opd1, vector<float> &opd2){
	if((result.size != opd1.size) ||
		(result.size != opd2.size) ||
		(opd1.size != opd2.size)){
		cout << "ERROR: Vector sizes did not match in add vector" << endl;
		exit(1);
	}
	for(int i = 0; i < result.size; i++){
		result[i] = opd1[i] + opd2[i];
	}
}


int main(int argc, char*argv[])
{
	state_parameters = new state();

	for(int i = 0; i < 1000; i++){
		float gait_phase = state_parameters->get_gait_phase();

		vector<float> tau_pd(state_parameters->get_number_joints());
		vector<float> tau_vf(state_parameters->get_number_joints());
		vector<float> tau_net(state_parameters->get_number_joints());

		compute_tau_pd(tau_pd);
		compute_tau_vf(tau_vf);

		add_vector(tau_net, tau_pd, tau_vf);
		cout << "Time = " << i << endl;
	}

	return 0;
}
