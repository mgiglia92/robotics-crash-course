#include <MPU6050.h>
//#include <Integrators.h>

MPU6050 IMU(4,5);
double wz;
double wz_hat;
double yaw = 0;
double yaw_hat;
unsigned long current_micros = 0;
unsigned long prev_micros = 0;
double sample_period = 0.05; //sampling period in seconds
double dt; //current delta t

void setup() {
  Serial.begin(115200);
  IMU.initialize();
  IMU.calibrate();
}

void loop() {
  //Update current time
  current_micros = micros();

  //get dt
  dt = (current_micros - prev_micros)/1000000.0;
  

  //Check sampling time
  if(dt >= sample_period){
  //get ang vel from gyro
  IMU.update();
  wz = IMU.get_ang_vel('z');

  //get filtered value
  yaw_hat = kalman(yaw);

  //Integrate ang_vel to get yaw
  yaw = integrate(wz, yaw_hat, dt);

  serial_plotter(yaw, yaw_hat);

  //update prev_micros
  prev_micros = current_micros;
  }
  
}

double kalman(double u){
  static const double R = 50; //noise covariance
  static const double H = 1.00; //measurement map scalar
  static double Q = 10; //initial estimate covariance
  static double P = 0; //initial error covariance
  static double u_hat = 0; //initial estimated state
  static double K = 0; //initial kalman gain

  //begin calculations
  K = P*H / (H*P*H + R);
  u_hat = u_hat + K*(u-H*u_hat); //update estimated

  //update error covariance
  P = (1 - K*H)*P + Q;

  //return estimate u_hat
  return u_hat;
}

void serial_plotter(double raw, double estimate){
  Serial.print(raw); Serial.print(','); Serial.println(estimate);
}

double integrate(double x_dot, double x, double dt){
  return x + (x_dot*dt);
}
