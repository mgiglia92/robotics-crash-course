/*
 * 1-detlas.ino -- odom delta class
 *
 * Our encoder library always returns absolute encoder counts, much like
 * how millis() and micros() return absolute time.  Although absolute
 * values are great as they arguably give the most flexibility to the
 * programmer, constantly having to determine encoder deltas can be an
 * annoyance.  To get around this, create a wrapper class for the Odom
 * class found in the RCC library.  Your class should keep previous
 * encoder counts private.  Both the left and right encoder counts
 * should have their respective methods to return count deltas.  The
 * class constructor should also take an Odom reference as in input
 * parameter so that a private Odom reference can be used throughout the
 * class.  Such a design decision will allow for multiple instances of
 * the wrapper class to exist to return count deltas for different
 * purposes.
 */
