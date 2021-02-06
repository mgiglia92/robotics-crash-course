#include <MPU6050.h>
#include <math.h>

MPU6050 IMU(4,5);
unsigned long dt = 0.05;
unsigned long prev_micros = 0;
unsigned long current_micros = 0;

double alpha = 0.1;

double acc[] = {0, 0, 0}; //Raw acceleration values
double omega[] = {0, 0, 0};// Raw angular velocities 
double orientation_world_estimate[] = {0.0,0.0,0.0};
double o_plus[] = {0, 0, 0};
double ori_acc[] = {0, 0, 0};
double w_world[] = {0, 0, 0};

void setup(){
  IMU.initialize();
  IMU.calibrate();
  Serial.begin(115200);
}

void loop(){
  //Update IMU
  IMU.update();

  //Update time
  current_micros = micros();
  
  // Make sure we're calculating at delta T
  if(current_micros - prev_micros >= dt*1000000.0){
    
    // Get accel and gyro values
    acc[0] = IMU.get_accel('x');
    acc[1] = IMU.get_accel('y');
    acc[2] = IMU.get_accel('z');
    omega[0] = IMU.get_ang_vel('x')*PI/180;
    omega[1] = IMU.get_ang_vel('y')*PI/180;
    omega[2] = IMU.get_ang_vel('z')*PI/180;
  
    // Calculate complementary roll
    calc_roll_estimate(orientation_world_estimate, acc, omega, dt);
    orientation_world_estimate[0] = o_plus[0];
    orientation_world_estimate[1] = o_plus[1];
    orientation_world_estimate[2] = o_plus[2];
//  
    //Reset necessary params
    prev_micros = current_micros;

    // Printsome stuff
//    print_vector(orientation_world_estimate);
  }
}

void calc_roll_estimate(double* o, double* a, double* w, double delta_t){
  calc_acc_estimate(a);
  transform_to_world_frame(o, w);
  o_plus[0] = (1-alpha) * (o[0] + (w_world[0] * delta_t)) + (alpha * ori_acc[0]);
  o_plus[1] = (1-alpha) * (o[1] + (w_world[1] * delta_t)) + (alpha * ori_acc[1]);
  o_plus[2] = 0;// (1-alpha) * (o[2] + (w_world[2] * delta_t)) + (alpha * ori_acc[2]);
  print_vector(ori_acc);
}

void calc_acc_estimate(double* a){
  double ax = a[0];
  double ay = a[1];
  double az = a[2];
  ori_acc[0] = atan((ay / (sqrt(pow(ax,2) + pow(az,2)))));
  ori_acc[1] = atan((ax / (sqrt(pow(ay, 2) + pow(az, 2)))));
  ori_acc[2] = 0;
}

void transform_to_world_frame(double* o, double* w){
  w_world[0] = w[0] + sin(o[0])*tan(o[1])*w[1] + cos(o[0])*tan(o[2])*w[2];
  w_world[1] = cos(o[0])*w[1] - sin(o[0])*w[2];
  w_world[2] = sin(o[0])*(1/cos(o[1]))*w[1] + cos(o[0])*(1/cos(o[1]))*w[2];
}

void print_vector(double* vec){
  Serial.print("x: "); Serial.print(vec[0]); 
  Serial.print(" y: "); Serial.print(vec[1]);
  Serial.print(" z: "); Serial.println(vec[2]);
}
