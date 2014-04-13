#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include "gaitGraph.h"
#include "footPlacement.h"

using namespace std;

class controller
{
    private:

    public:
        gaitGraph * gait_graph;
        footPlacement * foot_controller;
        controller();
};

#endif // CONTROLLER_H