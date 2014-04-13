#ifndef ANGLE_H
#define ANGLE_H

#include <iostream>

using namespace std;

class angle
{
    private:
    	int current_value;
    	int upper_limit;
    	int lower_limit;

    public:
        angle(int upper_limit, int lower_limit);

        int get_value();
        int set_value(int value);

        void set_upper_limit(int value);
        void set_lower_limit(int value);
};

#endif // ANGLE_H