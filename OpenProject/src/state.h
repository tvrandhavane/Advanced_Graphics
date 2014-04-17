#ifndef STATE_H
#define STATE_H

#include "bonelink.h"
#include <iostream>

using namespace std;

/*
front left  0
front right 1
rear left 2
rear right 3
*/


class state
{
    private:

    public:

        MatrixFunc function;



        //General
        int number_joints;
        float curr_velocity[3];
        float target_velocity[3];

    	//Gait graph parameters
    	int stride_period;
    	int gait_phase;
    	int swing_start[4];
    	int swing_end[4];


        //foot
        float foot_location[3];
        //Back 
        bonelink pelvis ;
        bonelink spine1;
        bonelink spine2;
        bonelink spine3;
        bonelink spine4;
        bonelink scapula ;


        /*** Rear legs***/
        //Rear Right leg 
        bonelink Rfemur;
        bonelink Rtibia;
        bonelink Rmetartasus;
        bonelink R_rphalange;

        //Rear Left leg ;
        bonelink Lfemur;
        bonelink Ltibia;
        bonelink Lmetartasus;
        bonelink L_lphalange;

        //Front Right leg;
        bonelink Rhemurus ;
        bonelink Rulna;
        bonelink Rmetacarpus;
        bonelink R_fphalange;

        //Front Left leg
        bonelink Lhemurus ;
        bonelink Lulna;
        bonelink Lmetacarpus;
        bonelink L_fphalange;

        //tail
        bonelink tail1;
        bonelink tail2;     P1 = state_parameters->get_foot_location();
        P2 = Plf + (curr_velocity - target_velocity)*sfp;
        bonelink tail3;
        bonelink tail4;

        //neck
        bonelink neck1;
        bonelink neck2;
        bonelink neck3;
        bonelink head;



    	//Foot placement parameters

        //getters
    	int get_stride_period() ;
        int get_number_joints();
        int get_gait_phase() ;
        float* get_foot_location();
        float* get_curr_velocity();
        float* get_target_velocity();

        //setters
        void set_gait_phase(int phase);
        void set_foot_location(float pos[3]);
        Matrix & transferFrame(Matrix& M,  float length, float angleX, float angleY, float angleZ);


        state();

};

#endif // STATE_H