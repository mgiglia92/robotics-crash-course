/*
 * 0-non-primitive.ino -- user-defined types
 *
 * Once we start designing a more complex system, it becomes harder and
 * harder to describe using just primitive data types.  To get around
 * this, most modern languages let us define our own types.  Once such
 * way of doing this in C/C++ is to define a struct.  With this in mind,
 * create a struct that can be used for timing events.  This struct
 * should at the very least store the previous time, along with a time
 * delta.  To make this more of a useful type, you should also add a
 * flag member that can be used to determine if a timing event is
 * enabled or disabled.
 *
 * Using this new type, create an array of four timing event structs and
 * assign random deltas, but only enable the first event.  Each time the
 * first event is triggered, one more event should be triggered, until
 * all event are enabled, at which point the original state should be
 * reverted and this pattern should repeat.  On top of this, for every
 * event that is triggered, you should print a corresponding message
 * over UART.
 */

// define your struct here

// next create an array of your timing structs here

void setup()
{
  Serial.begin(9600);

  // set the timing event deltas here
  // also enable just the first event
}

void loop()
{
  // loop through your timing events and print out a message related to
  // the timing event that was triggered
}
