#include <ControllerParams.h>
#include <MPU6050.h>
#include <PID_v1.h>

MPU6050 IMU(4,5);

ControllerParams cp(5);



void setup()
{
cp.kpr = 1;
cp.kir = 0;
cp.kdr = 0;
PID linear_controller(&cp.linear_input, &cp.linear_output, &cp.linear_setpoint, cp.kpl, cp.kil, cp.kdl, DIRECT);
PID rotation_controller(&cp.rotation_input, &cp.rotation_output, &cp.rotation_setpoint, cp.kpr, cp.kir, cp.kdr, DIRECT);
}

void loop()
{

}
