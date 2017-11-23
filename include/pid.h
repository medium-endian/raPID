#ifndef PID_H
#define PID_H

void calculate_PID_absolute(float pid_roll_setpoint, float measurement, float roll_rate);
void calculatePID_angular_rate(float pid_roll_setpoint, float measurement);

#endif // PID_H