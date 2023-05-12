/*
 * 1-conversions.ino -- unit conversion functions
 *
 * Unit conversions are something that are hard to get around in the
 * world of engineering.  Since we're going to be using an ultrasonic
 * sensor in the future, why don't you create some functions that can
 * convert a time duration to a distance?  The duration returned by our
 * ultrasonic sensor is in microseconds, but it is also the two-way
 * duration for the pulse (this means we need to divide our duration in
 * half when determining distance).  The speed of sound at room
 * temperature is about 346m/s, with this information in mind, create
 * two functions that take the two-way duration (unsigned long) in
 * microseconds and returns the distance (float) in meters and
 * centimeters.
 */
