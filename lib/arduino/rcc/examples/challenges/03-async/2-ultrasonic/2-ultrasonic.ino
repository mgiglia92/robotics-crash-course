/*
 * 2-ultrasonic.ino -- asynchronous ultrasonic timing
 *
 * The HC-SR04 is the ultrasonic sensor found on your robot.  Although
 * sending a pulse takes just a handful of microseconds, waiting for the
 * returning pulse duration can be in the range of milliseconds, which
 * is an unacceptable duration when trying to tend to other asynchronous
 * timing events.  To get around such a limitation, we can instead make
 * use of an interrupt to time the pulse duration.
 *
 * In essence all that needs to be done is to begin timing the pulse
 * duration when an interrupt pin is triggered by a RISING wave, and
 * then end timing when the same interrupt pin is triggered by a FALLING
 * wave.  Once timing is done, a flag should also be set to indicate the
 * pulse duration is ready to read for the user.  You should also create
 * a function that wraps around this flag, as you should have some
 * method of setting a timeout in microseconds as to deal with the case
 * where the sent pulse does not find its way back to the sensor.
 *
 * Since the echo pin of the HC-SR04 is connected to pin 4 of the
 * Arduino Uno Rev3, you'll need to make use of an external library
 * called PinChangeInterrupt found here:
 *
 * https://github.com/NicoHood/PinChangeInterrupt
 *
 * The three functions we're most interested in to achieve this are
 * attachPinChangeInterrupt(), digitalPinToPinChangeInterrupt(), and
 * getPinChangeInterruptTrigger().  Their usage is best explained in
 * documentation linked above.
 *
 * Before you do run off and start coding away, there are a few things
 * to keep in mind:
 *   * use micros() for timing as the durations we're dealing with
 *     require far more precision than a millisecond
 *   * since we need to detect BOTH a RISING and FALLING edge, we'll
 *     need to trigger an interrupt on a CHANGE, as only one function
 *     can be registered for a pin change interrupt
 *   * a pulse should last about 10 microseconds, for this it is okay to
 *     use delayMicroseconds(), and for this duration, the trigger pin
 *     should be held HIGH
 *   * when checking if your pulse has timed out, make sure to disable
 *     and re-enable interrupts as some variables can be changed
 *     unexpectedly
 *   * ECHO_PIN: 4, TRIG_PIN: 10 (optionally use RCC_ECHO_PIN and
 *     RCC_TRIG_PIN from rcc.h instead)
 */
