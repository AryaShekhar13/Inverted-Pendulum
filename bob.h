#pragma once

class Pendulum {
public:
    float bobX;
    float bobY;
    float cartX;
    float Theta;
};

extern Pendulum state;
static const double PI = 3.141592653589793;
void updatePhysics(double dt);