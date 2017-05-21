#include "PID.h"

using namespace std;

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_init, double Ki_init, double Kd_init) {
    Kp = Kp_init;
    Ki = Ki_init;
    Kd = Kd_init;
    p_error = i_error = d_error = 0.0;
}

void PID::UpdateError(double cte) {
    // NB: The d_error has to be set before p_error so p_error
    // can act as the cte value from the previous iteration.
    d_error = cte - p_error;
    p_error = cte;
    i_error += cte;
    cout << "p_error: " << p_error << endl;
    cout << "i_error: " << i_error << endl;
    cout << "d_error: " << d_error << endl;
}

double PID::TotalError() {
    cout << "Kp, Ki, Kd: " << Kp << ", " << Ki << ", " << Kd << endl;
    double control_error = - (Kp * p_error + Ki * i_error + Kd * d_error);
    cout << "control_error: " << control_error << endl;
    return control_error;
}
