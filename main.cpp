#include <iostream>
#include <thread>
#include <conio.h>
#include <cmath>

using namespace std;

int main(){

    //Parameters
    double M = 10.0;
    double m = 1.0;
    double l = 10.0;
    double F = 0;
    const double g = 9.81;

    double x = 0;
    double x_d = 0;
    double x_dd = 0;

    double theta = 0.05;
    double theta_d = 0;
    double theta_dd = 0;

    char ch;
    const double dt = 0.05;   

    double k1 = -2.23606798;
    double k2 = -11.43582202;
    double k3 = -393.02754596;
    double k4 = -386.7813713;

    //coords
    double X = 0;
    double Y = l;

    //main loop
    while (true) {

        //eqns motion

        F = -1*(k1*x + k2*x_d + k3*theta + k4*theta_d);

        x_dd = (F + (m*l*theta_d*theta_d*sin(theta)) - m*g*sin(theta)*cos(theta))/(M + m - m*cos(theta)*cos(theta));
        theta_dd = ((g*sin(theta) - x_dd*cos(theta))/l);

        x_d += x_dd*dt;
        x += x_d*dt;

        theta_d += theta_dd*dt;
        theta += theta_d*dt;

        X = x + l * sin(theta);
        Y = l * cos(theta);

    if (isnan(theta)) {
        cout << "theta became NaN" <<endl;
        break;
    }

    if (isnan(theta_dd)) {
        cout << "theta_dd became NaN\n" <<endl;
        break;
    }

    if (isnan(x_dd)) {
        cout << "x_dd became NaN\n" <<endl;
        break;
    }

    if (abs(theta) < 0.001 &&
        abs(theta_d) < 0.001 &&
        abs(x_d) < 0.001){
        cout << "Balanced!\n";
        break;
    }

        cout<<"X coordinate: "<< X <<" "<<"Y coordinate: "<< Y <<" "<<"Force: "<< F <<endl;
    }
}