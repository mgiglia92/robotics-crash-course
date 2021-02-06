//Estimate orientation by fusing encoders and gyro
#include <MPU6050.h>
#include <HC_SR04.h>
#include <math.h>
#include "PinChangeInterrupt.h"
#include "Differentiator.h"
#include <motor_control.h>
#include <PID_beard.h>

#define TRIG_PIN 10
#define ECHO_PIN 2
#define ECHO_INT 0
#define RIGHT_ENCODER 13
#define LEFT_ENCODER 4
#define SERVO_PIN
#define PPR 20
#define WHEEL_DIAMETER 0.08 //meters
#define WHEEL_BASE 0.13208 //meters

//System parameter variables
volatile int right_count = 0; //Emcoder counts on wheels
volatile int left_count = 0;
double omega_r; //Angular velocity of wheels
double omega_l;

// Rigid body system state variables
double yaw=0;
double yaw_dot;

double v_car_encoders; //Velocity of the car from encoder calculations
double v_car_accel; //velocity of the car from acceleration integration
double s[3]; //Position of the car in global coordinates
double v[3]; //Velocity of the car in global coordinates
double acc[] = {0, 0, 0}; //Raw acceleration values
double omega[] = {0, 0, 0};// Raw angular velocities 
double orientation_world_estimate[] = {0.0,0.0,0.0};
double o_plus[] = {0, 0, 0};
double ori_acc[] = {0, 0, 0};
double w_world[] = {0, 0, 0};

//Estimated state variables
double yaw_dot_hat;
double s_hat[3];
double v_hat[3];
double v_car_hat;

//Control input variables
static double pid_out_vel;
static double pid_out_yaw;
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

//PID Controller instantiation
PIDControl velocity_controller(20, 10, 0, 0, 255, sigma, dt, true);
PIDControl yaw_controller(50, 1, 1, -100, 100, sigma, dt, true);
  
// instantiate complementary filter ratio
double alpha_yaw = 0.05;
double alpha_velocity = 0.2;



void setup(){
  //Set controller params
  velocity_controller.anti_windup_activated = true; //activate integrator antiwindup
  velocity_controller.update_deadband_values(50, -50);
  yaw_controller.anti_windup_activated = true; //activate integrator antiwindup
  
  attachPCINT(digitalPinToPCINT(RIGHT_ENCODER), do_right_count, RISING);
  attachPCINT(digitalPinToPCINT(LEFT_ENCODER), do_left_count, RISING);

  motor_setup();
  
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
//GATHER SENSOR DATA-----------------------------------------------------------
    // Get accel and gyro values
    acc[0] = IMU.get_accel('x');
    acc[1] = IMU.get_accel('y');
    acc[2] = IMU.get_accel('z');
    omega[0] = IMU.get_ang_vel('x')*PI/180;
    omega[1] = IMU.get_ang_vel('y')*PI/180;
    omega[2] = IMU.get_ang_vel('z')*PI/180;

//CALCULATE STATE PARAMETERS----------------------------------------------------
    //Calculate differentials on wheel
    omega_r = drdt.differentiate(right_count*2*PI/PPR);
    omega_l = dldt.differentiate(left_count*2*PI/PPR);

    //Get system yaw from wheel velocities
    yaw_dot = encoders_to_yaw_dot(omega_r, omega_l);

    //Get system velocity from wheel velocities
    v_car_encoders = encoders_to_velocity(omega_r, omega_l);

//INTEGRATIONS AND STATE ESTIMATES----------------------------------------------------------
    //calc yaw state estimate
    yaw_dot_hat = complementary_filter(omega[2], yaw_dot, alpha_yaw);

    //Integrate acceleration along car's x-axis to get velocity
    v_car_accel = integrate(acc[0], v_car_hat, dt);
    
    //calc car velocity state estimate
    v_car_hat = complementary_filter(v_car_encoders, v_car_accel, alpha_velocity);

    //Integrate to get yaw
    yaw = integrate(yaw_dot_hat, yaw, dt);

    //Get world velocities from v_car_hat
    transform_vel_to_world(yaw, v_car_hat);

    //get world positions by integrating world velocities
    s[0] = integrate(v[0], s[0], dt);
    s[1] = integrate(v[1], s[1], dt);
        
    //Reset necessary params
    prev_micros = current_micros;

    // Printsome stuffS
//    print_vector(orientation_world_estimate);
//    Serial.print("wr: "); Serial.print(omega_r); Serial.print(" wl: "); Serial.print(omega_l); Serial.print(" yawdot: "); Serial.print(yaw_dot);
//    Serial.print(" gyro: "); Serial.println(omega[2]);
      Serial.print(pid_out_vel/255);
      Serial.print(',');
      Serial.print(pid_out_yaw/100);
      Serial.print(',');
      Serial.print(-1*yaw);
      Serial.print(',');
      Serial.println(0.5 - v_car_hat);
  }

  //Check if we've rotated 90deg, keep motor on if not

  pid_out_vel = velocity_controller.PID(0.5, v_car_hat);
  pid_out_yaw = yaw_controller.PID(0, yaw);
  pwr_left = pid_out_vel - pid_out_yaw;
  pwr_right = pid_out_vel + pid_out_yaw;

  //set motor power
  raw_motor_control(pwr_left, pwr_right);
}

double complementary_filter(double gyro, double model, double alpha){
  return ((1-alpha) * gyro) + (alpha * model);
}

void calc_roll_estimate(double* o, double* a, double* w, double delta_t){
  calc_acc_estimate(a);
  transform_to_world_frame(o, w);
  o_plus[0] = (1-alpha_yaw) * (o[0] + (w_world[0] * delta_t)) + (alpha_yaw * ori_acc[0]);
  o_plus[1] = (1-alpha_yaw) * (o[1] + (w_world[1] * delta_t)) + (alpha_yaw * ori_acc[1]);
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
  if(pwr_right >= 0){ right_count++;}
  else if(pwr_right <0) {right_count--;}
}
 void do_left_count(){
  if(pwr_left >= 0){ left_count++;}
  else if(pwr_left <0) {left_count--;}
 }

//-----------------------------------------------------------------------
//Robot yaw rotation model
//wr is the angular velocity of the right wheel, wl the left
double encoders_to_yaw_dot(double wr, double wl){
  return (wr - wl) * WHEEL_DIAMETER / WHEEL_BASE;
}

// Get the car's local velocity vector from wheel angular velocities
double encoders_to_velocity(double wr, double wl){
  return WHEEL_DIAMETER * (wr + wl) / 2;
}

// Pass in car's velocity local coordinates, convert to world coordinates
void transform_vel_to_world(double yaw, double vel){
  v[0] = vel * cos(yaw);
  v[1] = vel * sin(yaw);
}

double integrate(double x_dot, double x, double dt){
  return x + (x_dot*dt);
}
