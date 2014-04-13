#ifndef GAITGRAPH_H
#define GAITGRAPH_H

#include <iostream>
#include "state.h"

using namespace std;

extern state * state_parameters;

/*
front left  0
front right 1
rear left 2
rear right 3
*/

class gaitGraph
{
    private:


    public:
        gaitGraph(int stride_period);

        int get_stride_period();
        float get_gait_phase();

        void set_stride_period(int stride_period);

        void set_swing_start(int start_time, int leg);
        void set_swing_end(int start_time, int leg);

};

#endif // GAITGRAPH_H