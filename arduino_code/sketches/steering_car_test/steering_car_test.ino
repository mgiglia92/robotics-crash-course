#include <PID_beard.h>
#include <AsyncTimer.h>
#include <Differentiator.h>

//parameter stuff
#define PPR 96.0
#define PPR_THROTTLE 20.0
#define GEAR_RATIO 9.68
#define REAR_DRIVE_RATIO 1.0
#define WHEEL_DIAM 80.0 //mm

//pin stuff
#define ENA 11
#define IN1 12
#define IN2 13
#define ENB 9
#define IN3 8
#define IN4 7
#define ENC_A 2
#define ENC_B 3
#define ENC_THROTTLE 18

volatile int count=0;
volatile int count_throttle=0;
int prev_count_throttle=0;
double omega;

double kp = 60;
double ki = 10;
double kd = 1;
double llim = -255;
double ulim = 255;
double sigma = 0.1;
double sigma_throttle = 0.5;
double Ts = 0.01;
PIDControl steering_control(kp, ki, kd, llim, ulim, sigma, Ts, true);
PIDControl throttle_control(20, 10, 1, 0.0, ulim, sigma_throttle, Ts, true);
Differentiator throttle_diff(sigma_throttle, Ts);
double desired_angle = 8;
double desired_omega = 50;
double current_angle;
double controller_output;
double throttle_output;
double freq = .5;

AsyncTimer timer(Ts, 0.0, 0.0);

void setup() {
  // put your setup code here, to run once:
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_THROTTLE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), do_count_a, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_B), do_count_b, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_THROTTLE), do_count_throttle, RISING);
  steering_control.update_deadband_values(50, -50);
  steering_control.anti_windup_activated = true;
  throttle_control.anti_windup_activated = true;
  calibrate();
  //print stuff
  Serial.begin(115200);
  Serial.print("desired"); Serial.print(',');
  Serial.print("actual"); Serial.print(',');
  Serial.print("omega"); Serial.print(',');
  Serial.print("count"); Serial.print(',');
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.check_dt();
  update_system();
  do_controls();
  //print stuff
  Serial.print(desired_angle); Serial.print(',');
  Serial.print(current_angle); Serial.print(',');
  Serial.print(count); Serial.print(',');
//  Serial.print(0.0); Serial.print(',');
  Serial.println();
  
}

void update_system()
{
  current_angle = ((count / PPR) / GEAR_RATIO)*360.0;
//  desired_angle = 10*sin(2*PI*freq*timer.current/1000.0);
  
}

void do_controls()
{
  //Check if Ts time has passed for process 0
  if(timer.run_process[0] == true)
  {
    //Update variables that are time sensitive
    omega = throttle_diff.differentiate(count_throttle);
    //Calculate steering output
    controller_output = steering_control.PID(desired_angle, current_angle);
//    controller_output  = 0.0;
    //Calculate velocity motor output
    throttle_output = throttle_control.PID(desired_omega, omega);
//    throttle_output = 0.0;
  }
  control_motors(controller_output, throttle_output);
}

void control_motors(double pwr_steer, double pwr_throttle)
{
  if(pwr_steer <= 0)
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, abs(pwr_steer));
  }
  else{
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, abs(pwr_steer));
  }
  if(pwr_throttle <= 0)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 0);
  }
  else{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, abs(pwr_throttle));
  }
}

//Run calibration routine on steering system
void calibrate()
{
  //Turn motor ccw    
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, abs(200));
    delay(1500);
    //get count_ccw
    int count_ccw = count;

  //turn motor cw    
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, abs(200));
    delay(1500);
    //get count_cw
    int count_cw = count;

  //Reset count to what it should be at cw end stop
  int delta_count = count_ccw - count_cw;
  count = (int)(delta_count/2.0)*-1.0;
  
}

void do_count_a()
{
  if(digitalRead(ENC_B) == HIGH)  { count++ ; }//counterclockwise//count keeps increasing as moving in the same direction
  else { count--; } 
  //"count" counts number of revolutions so gives us the change of theta 
}

void do_count_b()
{
  if(digitalRead(ENC_A) == LOW)  { count++ ; }//counterclockwise//count keeps increasing as moving in the same direction
  else { count--; } 
  //"count" counts number of revolutions so gives us the change of theta 
}

void do_count_throttle()
{
  count_throttle ++;
}
