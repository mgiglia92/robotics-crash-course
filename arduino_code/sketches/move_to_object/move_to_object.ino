//Estimate orientation by fusing encoders and gyro
#include <MPU6050.h>
#include <HC_SR04.h>
#include <math.h>
#include "PinChangeInterrupt.h"
#include "Differentiator.h"
#include "motor_control.h"

#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0
#define RIGHT_ENCODER 13
#define LEFT_ENCODER 4
#define SERVO_PIN
#define PPR 20
#define WHEEL_DIAMETER 0.08 //meters
#define WHEEL_BASE 0.13208 //meters

//System state related variables
volatile int right_count = 0;
volatile int left_count = 0;
double omega_r;
double omega_l;
double yaw=0;
double yaw_dot;
double yaw_dot_hat;

//Motor power variables
int pwr_right = 0;
int pwr_left = 0;

// Instantiate sensors
MPU6050 IMU(4,5);
HC_SR04 dist(TRIG_PIN, ECHO_PIN, ECHO_INT);

// Instantiate timing variables
double dt = 0.05;
unsigned long prev_micros = 0;
unsigned long current_micros = 0;
double sigma = 0.1;

// Instantiate differentiators
Differentiator drdt(sigma, dt);
Differentiator dldt(sigma, dt);

// instantiate complementary filter ratio
double alpha = 0.05;

// instantiate state variable vectors
double acc[] = {0, 0, 0}; //Raw acceleration values
double omega[] = {0, 0, 0};// Raw angular velocities 
double orientation_world_estimate[] = {0.0,0.0,0.0};
double o_plus[] = {0, 0, 0};
double ori_acc[] = {0, 0, 0};
double w_world[] = {0, 0, 0};

void setup(){
  attachPCINT(digitalPinToPCINT(RIGHT_ENCODER), do_right_count, RISING);
  attachPCINT(digitalPinToPCINT(LEFT_ENCODER), do_left_count, RISING);
  
  IMU.initialize();
  IMU.calibrate();
  dist.begin();
  Serial.begin(115200);
}

void loop(){
  //Update IMU
  IMU.update();

  //Update time
  current_micros = micros();
  
  // Make sure we're calculating at delta T intervals
  if(current_micros - prev_micros >= dt*1000000.0){

    //Calculate differentials on wheel
    omega_r = drdt.differentiate(right_count*2*PI/PPR);
    omega_l = dldt.differentiate(left_count*2*PI/PPR);

    //Get system yaw from wheel velocities
    yaw_dot = encoders_to_yaw_dot(omega_r, omega_l);
    
    // Get accel and gyro values
//    acc[0] = IMU.get_accel('x');
//    acc[1] = IMU.get_accel('y');
//    acc[2] = IMU.get_accel('z');
//    omega[0] = IMU.get_ang_vel('x')*PI/180;
//    omega[1] = IMU.get_ang_vel('y')*PI/180;
    omega[2] = IMU.get_ang_vel('z')*PI/180;
  
    // Calculate complementary roll
//    calc_roll_estimate(orientation_world_estimate, acc, omega, dt);
//    orientation_world_estimate[0] = o_plus[0];
//    orientation_world_estimate[1] = o_plus[1];
//    orientation_world_estimate[2] = o_plus[2];

    //calc state estimate
    yaw_dot_hat = complementary_filter(omega[2], yaw_dot);

    //Integrate to get yaw
    yaw = integrate(yaw_dot_hat, yaw, dt);
    
    //Reset necessary params
    prev_micros = current_micros;

    // Printsome stuffS
//    print_vector(orientation_world_estimate);
//    Serial.print("wr: "); Serial.print(omega_r); Serial.print(" wl: "); Serial.print(omega_l); Serial.print(" yawdot: "); Serial.print(yaw_dot);
//    Serial.print(" gyro: "); Serial.println(omega[2]);
      Serial.print(yaw_dot);
      Serial.print(',');
      Serial.print(omega[2]);
      Serial.print(',');
      Serial.print(yaw_dot_hat);
      Serial.print(',');
      Serial.println(yaw);
  }

  //Check if we've rotated 90deg, keep motor on if not
  if(yaw < (PI*2)){pwr_right = 150; pwr_left = -150;}
  else { pwr_right = 0; pwr_left = 0; }

  //set motor power
  raw_motor_control(pwr_left, pwr_right);
}

double complementary_filter(double gyro, double model){
  return ((1-alpha) * gyro) + (alpha * model);
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

//---------------------------------------------------------------------------------------------------------------
//Interrupt Functions
void do_right_count(){
  if(pwr_right > 0){ right_count++;}
  else if(pwr_right >0) {right_count--;}
}
 void do_left_count(){
  if(pwr_left > 0){ left_count++;}
  else if(pwr_left >0) {left_count--;}
 }

//-----------------------------------------------------------------------
//Robot yaw rotation model
//wr is the angular velocity of the right wheel, wl the left
double encoders_to_yaw_dot(double wr, double wl){
  return (wr - wl) * WHEEL_DIAMETER / WHEEL_BASE;
}

double integrate(double x_dot, double x, double dt){
  return x + (x_dot*dt);
}
