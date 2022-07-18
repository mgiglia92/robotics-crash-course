/*
 * 2-async-delay.ino -- async delay class
 *
 * Checking a timer and updating the appropriate timer variables is a
 * very common task, as such it would be nice to encapsulate such
 * functionality in the form of an asynchronous delay class.  How you
 * wish to implement such a class is up to you, but make sure to have at
 * least once method which will return a boolean for if a timing event
 * is ready.  Internally this method should update all the appropriate
 * timing variables as to prime the asynchronous delay for the next
 * call.  This method should take one input parameter, and that is how
 * much time you'd wish to delay by (unsigned long).  By doing it in
 * this manner, you actually have enough information to figure out if
 * the delay value changed (and whether to start a delay from scratch).
 *
 * Although not required, it might be nice to add functionality to
 * specify the timer function we'd like to use (either millis() or
 * micros()), to do this you'll need to pass in a function.  Since
 * function pointers have yet to be explained, you'll need to define a
 * variable with the following signature:
 *
 * unsigned long (*timer)(void)
 *
 * Inside your class you'd just call the function like so: timer() and
 * it would work as either millis() or micros(), depending on which was
 * specified to the constructor.  To garuntee that timer() can indeed be
 * called, your constructor should mandate a timer function be
 * specified.
 */
