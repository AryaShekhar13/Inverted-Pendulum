#include <iostream>
#include <thread>
#include <conio.h>
#include <cmath>

using namespace std;

int main(){
    //Parameters
    double M = 10.0;
    double m = 1.0;
    double l = 1.0;
    double F = 0;
    const double g = 9.81;

    double x = 0;
    double x_d = 0;
    double x_dd = 0;

    const double PI = 3.141592653589793;
    double theta = PI/2+PI/3;
    double theta_d = 0;
    double theta_dd = 0;

    char ch;
    const double dt = 0.01;   

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

    //main loop
     while (true) {

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