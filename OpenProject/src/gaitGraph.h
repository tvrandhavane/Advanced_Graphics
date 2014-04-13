#ifndef GAITGRAPH_H
#define GAITGRAPH_H

#include <iostream>
using namespace std;

/*
front left  0
front right 1
rear left 0
rear right 1
*/

class gaitGraph
{
    private:
    	int stride_period;
    	int gait_phase;
    	int swing_start[4];
    	int swint_end[4];

    public:
        gaitGraph();

        int get_stride_period();
        float get_gait_phase();

        void set_stride_period(int stride_period);

        void set_swing_start(int start_time, int leg);
        void set_swing_end(int start_time, int leg);

};

#endif // GAITGRAPH_H