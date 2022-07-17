/*
 * 2-stack-overflow.ino -- stack overflow
 *
 * A recursive function is a function that calls itself.  A famous
 * example of a recursive function is the Fibonacci sequence.  With this
 * in mind, create a function that gives you the Nth Fibonacci number
 * by writing a recursive function.  This function should return the
 * result as an unsigned integer and the input parameter should also be
 * an unsigned integer.
 *
 * Right when you enter your function you should also print out the
 * memory address and the value of the input parameter with the
 * following bit of code (assuming the input parameter is called n):
 *
 *  Serial.print("a:");
 *  Serial.print((uintptr_t) &n);
 *  Serial.print(": ");
 *  Serial.println(n);
 *
 * Finally, run your function in setup() with various input values.
 * What happens when N is 10? 100? 1000? 10000?
 *
 * NOTE: the output you will see over UART will depend heavily on how
 * the compiler decides to organize memory.
 */
