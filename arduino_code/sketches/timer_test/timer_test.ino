// Timer Test
#import <motor_control.h>
#import <Timer.h>

Timer forwards_timer(2000);
Timer backwards_timer(1000);

void setup() {
  forwards_timer.start();
  Serial.begin(9600);
}

float old_millis = millis();

void loop() {
  Serial.println(millis() - old_millis);
  
  if(forwards_timer.is_finished(false)){
    backward(150);
    backwards_timer.start();
    old_millis = millis();
  }

  if(backwards_timer.is_finished(false)){
    forward(150);
    forwards_timer.start();
    old_millis = millis();
  }

}
