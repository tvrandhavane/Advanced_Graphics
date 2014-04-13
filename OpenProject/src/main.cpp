#include <iostream>
#include "controller.h"

using namespace std;


int main(int argc, char*argv[])
{
	controller * myController = new controller();
	myController->set_stride_period(10);

	return 0;
}
