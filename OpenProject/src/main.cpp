#include <iostream>
#include "controller.h"
#include "state.h"

using namespace std;

state * state_parameters;

int main(int argc, char*argv[])
{
	state_parameters = new state();
	controller * myController = new controller();
	cout << myController->gait_graph->get_stride_period() << endl;

	return 0;
}
