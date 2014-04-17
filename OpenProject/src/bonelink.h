#ifndef BONELINK_H
#define BONELINK_H

#include "angle.h"
#include "MatrixFunc.h"
#include <iostream>

class bonelink{

public:
	float angleX;
	float angleY;
	float angleZ;
	bonelink();
	Matrix * Transfer;

	void set_Matrix();


};

#endif