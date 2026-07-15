#include <iostream>
#include <thread>
#include <conio.h>
#include <cmath>
#include "bob.h"

using namespace std;

void updatePhysics(double dt){
    //Parameters
    double M = 10.0;
    double m = 1.0;
    double l = 1.0;
    double F = 0;
    const double g = 9.81;

    static double x = 0;
    static double x_d = 0;
    static double x_dd = 0;

    static double theta = PI/2+PI/3;
    static double theta_d = 0;
    static double theta_dd = 0;

    char ch;  

    double k1 = -2.23606798;
    double k2 = -8.43909272;
    double k3 = -262.37833267;
    double k4 = -80.58232035;

    double kE = 15.0;
    double kv = 1.5;
    double kx = 3.0;

    //coords
    double X = 0;
    double Y = l;

        //eqns motion

        if(abs(theta) < 0.2 && abs(theta_d) < 1.0){
            //lqr
            F = -1*(k1*x + k2*x_d + k3*theta + k4*theta_d);
        }else{
            //PD controller
            F = kE*((0.5*m*l*l*theta_d*theta_d)+m*g*l*(1+cos(theta))-2*m*g*l)*theta_d*cos(theta)- kv*x - kx*x_d;
        }
        
        x_dd = (F + (m*l*theta_d*theta_d*sin(theta)) - m*g*sin(theta)*cos(theta))/(M + m - m*cos(theta)*cos(theta));
        theta_dd = ((g*sin(theta) - x_dd*cos(theta))/l);

        x_d += x_dd*dt;
        x += x_d*dt;

        theta_d += theta_dd*dt;
        theta += theta_d*dt;

while (theta > PI)
    theta -= 2 * PI;

while (theta < -PI)
    theta += 2 * PI;

        X = x + l * sin(theta);
        Y = l * cos(theta);
        float CoordX = X;
        float CoordY = Y;
        float cartX = x;
        float Theta = theta;
        state.bobX = CoordX;
        state.bobY = CoordY;
        state.cartX = cartX;
        state.Theta = Theta;

}