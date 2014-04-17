#ifndef STATE_H
#define STATE_H

#include <iostream>

using namespace std;

/*
front left  0
front right 1
rear left 2
rear right 3
*/
struct bone {
    float link_length ;
    float angleX ;
    float angleY ;
    float angleZ ;

};


class state
{
    private:

    public:
        //General
        int number_joints;


    	//Gait graph parameters
    	int stride_period;
    	int gait_phase;
    	int swing_start[4];
    	int swing_end[4];


        //Back 
        bone pelvis ;
        bone spine[4];
        bone scapula ;


        /*** Rear legs***/
        //Rear Right leg 
        bone Rfemur;
        bone Rtibia;
        bone Rmetartasus;
        bone R_rphalange;

        //Rear Left leg ;
        bone Lfemur;
        bone Ltibia;
        bone Lmetartasus;
        bone L_lphalange;

        //Front Right leg;
        bone Rhemurus ;
        bone Rulna;
        bone Rmetacarpus;
        bone R_fphalange;

        //Front Left leg
        bone Lhemurus ;
        bone Lulna;
        bone Lmetacarpus;
        bone L_fphalange;

        //tail
        bone tail[4];

        //neck
        bone neck[3];
        bone head;



    	//Foot placement parameters
    	int get_stride_period() ;
        int get_number_joints();
        int get_gait_phase() ;
        state();

};

#endif // STATE_H