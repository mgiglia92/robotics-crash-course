/*
 * 1-interrupts.ino -- using interrupts
 *
 * Since our robot has simple encoders, we cannot tell if a wheel is
 * rotating forwards or backwards so it becomes the job of us, the
 * programmer, to keep track of the direction we are moving.
 *
 * In the RCC library, there is just a global variable that is
 * incremented with each encoder tick, regardless of rotation direction.
 * This encoder count is absolute, and it is the job of the programmer
 * to make use of this data.  Although this is probably the most
 * versatile implementation, an equally valid approach can be returning
 * a signed amount of ticks that have occurred since the last time a
 * reading was requested.
 *
 * With this in mind, create an ISR for each encoder that will
 * increment/decrement a tick variable (signed long) depending on a
 * direction flag for each wheel.  There should be a wrapper function
 * for each encoder that returns the ticks, and then sets the tick count
 * to zero.  Make sure to disable interrupts when setting this value
 * back to zero to ensure that this operation is atomic (making it so
 * that an ISR does not unintentionally overwrite this midway through as
 * it takes multiple instructions to modify a 32-bit integer on an 8-bit
 * microcontroller), and to then re-enable interrupts when the operation
 * is complete.
 */
