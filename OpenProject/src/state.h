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

class state
{
    private:

    public:
    	//Gait graph parameters
    	int stride_period;
    	int gait_phase;
    	int swing_start[4];
    	int swint_end[4];

    	//Foot placement parameters
    	

        state();
};

#endif // STATE_H