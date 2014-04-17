#include <iostream>
#include <vector>
#include <cstdlib>
#include "controller.h"
#include "state.h"

using namespace std;

enum leg_frame
{
	shoulders,
	hip
};

state * state_parameters;
void compute_tau_pd(vector<float> &tau);
void compute_tau_vf(vector<float> &tau);
void add_vector(vector<float> &result, vector<float> &opd1, vector<float> &opd2);
void print_vector(vector<float> &vec);
void get_tau_stance(leg_frame lf, bool l_stance, bool r_stance, vector<float> &vec);
void foot_placement_control(float swing_phase);
void compute_fv(float &fv);


void foot_placement_control(float swing_phase, float & new_Pos[3]){
	float P1[3], P2[3];
	float curr_V =  state_parameters->get_curr_velocity();
	float target_V =  state_parameters->get_target_velocity();
	P1 = state_parameters->get_foot_location();
	if(swing_phase == 0){
		P2[0] = Plf + (curr_V[0] - target_V[0])*sfp;
		P2[1] = Plf + (curr_V[1] - target_V[1])*sfp;
		P2[2] = Plf + (curr_V[2] - target_V[2])*sfp;
		state_parameters->set_foot_location(P2);
	}
	else{
		P2 = P1; 
	}
	for(int i =0 ;i < 3; i++){
		new_Pos[i] = swing_phase*P1[i] + (1 - swing_phase)*P2[i];
	}
	new_Pos[1] = height_trajectory(swing_phase);

}


void compute_tau_pd(vector<float> &tau, float gait_phase){
	//computation of target angle

	//head
		//state_parameters->get_head_position(gait_phase)
		//current angles = state_parameters->get_current_angle()
		//Apply Inverse Kinematics to get the angles of intermediate links
		//Thus we get the target angles

	//tail
		//target angles are given as state parameters

	//spine
		//difference between quaternions

	//legs
		//for each leg in swing phase
			//position = foot_placement_control(state_parameters->get_swing_phase(leg_id))
			//then IK to get angles for intermediate links

	/*
	for each joint i
		init_angle = current angle
		target_angle = target angle
		tau[i] = kp * (target_angle - init_angle) + kd * (target_angle - init_angle) / time_step
	*/
}

void compute_tau_vf(vector<float> &tau){
	float fv = 0;
	compute_fv(fv);
}

void add_vector(vector<float> &result, vector<float> &opd1, vector<float> &opd2){
	if((result.size() != opd1.size()) ||
		(result.size() != opd2.size()) ||
		(opd1.size() != opd2.size())){
		cout << "ERROR: Vector sizes did not match in add vector" << endl;
		exit(1);
	}
	for(int i = 0; i < result.size(); i++){
		result[i] = opd1[i] + opd2[i];
	}
}

void print_vector(vector<float> &vec){
	for(int i = 0; i < vec.size(); i++){
		cout << vec[i] << " ";
	}
	cout << endl;
}

void get_tau_stance(leg_frame lf, bool l_stance, bool r_stance, vector<float> &vec){

}

void compute_fv(vector<float> &fv){
	for(int i = 0; i < fv.size(); i++){
		fv[i] = kv*(state_parameters->vd[i] - state_parameters->v[i]);
	}
}

int main(int argc, char*argv[])
{
	state_parameters = new state();

	for(int i = 0; i < 2; i++){
		float gait_phase = (float)state_parameters->get_gait_phase()/
							(float)state_parameters->get_stride_period();

		vector<float> tau_pd(state_parameters->get_number_joints());
		vector<float> tau_vf(state_parameters->get_number_joints());
		vector<float> tau_net(state_parameters->get_number_joints());

		compute_tau_pd(tau_pd);
		compute_tau_vf(tau_vf);

		add_vector(tau_net, tau_pd, tau_vf);

		//
		cout << "tau_net = ";
		print_vector(tau_net);
		cout << "Time = " << i << endl;
		//

		get_tau_stance(shoulders, true, true, tau_net);
		get_tau_stance(hip, true, true, tau_net);

		state_parameters->set_gait_phase(state_parameters->get_gait_phase() + 1);
	}

	return 0;
}
