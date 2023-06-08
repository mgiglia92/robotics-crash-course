/*
 * 2-generalization.ino -- array generalization
 *
 * Arrays are one of the core foundations of modern programming, without
 * them, most of our computing solutions would not be possible!  Since
 * arrays play such a pivotal role in programming, it is important to
 * know how to write a code that can be generalized (i.e. a function
 * that can write the values of an N-length integer array over UART).
 *
 * If you've dabbled around a bit with C/C++ syntax you might be aware
 * that you can pass an N-length array into a function like so:
 *
 * void foo(int bar[], int length);
 *
 * Unfortunately, the syntax above does not work for statically
 * allocated arrays greater than dimension one.  To get around this we
 * must employ some new syntax, the gist of which can be summed up as
 * so:
 *
 * array[] == *array
 *
 * What the statement above is trying to convey is that these two are
 * the same way of describing the same datatype.  Now we can rewrite our
 * original example like so:
 *
 * void foo(int *bar, int length);
 *
 * Don't let this syntax deceive you!  If you were to statically
 * allocate a 2D array with dimensions N and M and then tried passing it
 * to a function that look like this:
 *
 * void foo(int **bar, int n, int m);
 *
 * you would get a compiler error complaining that your input argument
 * is of an invalid type!  This is because:
 *
 * array[][] != **array
 *
 * The short and slightly inaccurate explanation is that each asterisk
 * can only be related to a one dimensional array.
 *
 * So then how do we get around this?  Great question!  What if instead
 * of having a 2D array of dimensions N and M, we had N arrays of length
 * M, and an additional array of size N that contains the N, M-length
 * arrays?  Suddenly the syntax above is valid.
 *
 * With that said, take your 2D robot dance array and turn it into an
 * array of arrays.  Then create a generalized function that prints a 2D
 * array of dimensions N and M along with the value's respective index.
 * (Hint: your array of arrays will most likely be of type int *array[])
 *
 *
 * For that one person that wants to know why this works, firstly, you're
 * probably a really cool nerd, but secondly, this comes down to how
 * statically-allocated arrays are laid out in memory.  Let's take the
 * following 2D array into consideration:
 *
 * int foobar[2][3] = {
 *   {1, 2, 3},
 *   {4, 5, 6},
 * };
 *
 * it's actually laid out sequentially in memory like this:
 *
 * {{1, 2, 3}, {4, 5, 6}}
 *
 * Now the problem comes that an asterisk can only traverse an one-
 * dimensional array type.  So lets say we hypothetically did pass array
 * foobar into our foo() function from before.  If we were to do
 * bar[2][1], the CPU would first go to the digit 3 in the array, and
 * then try to get the 2nd element of the next array, but the problem
 * here is that 3 isn't an array, it's an int!
 *
 * Conversely, an array of arrays like this:
 *
 * int foobar0[] = {1, 2, 3};
 * int foobar1[] = {4, 5, 6};
 * int *foobar[] = {foobar0, foobar1};
 *
 * would still be laid out sequentially in memory, but now our first
 * array traversal would yield an array type!  Hence why this works.
 * This still isn't 100% true, but a bit of inaccuracy saves a world of
 * explanation and for the time being, this should suffice ;)
 */
