Small and simple implementations of various algorithms and data structures.
===========================================================================

(Extensive) function and macro documentation can be found in the headers in the
<b>include</b> directory. Tests are in the <b>tests</b> directory. 
Implementations in the <b>src</b> directory.

The code makes use of C99/11 features, including variadic macros, stdbool.h,
and alignas() (for the arena allocator).

Running:

    $ make
    $ ./test

The entry point is in <b>tests/test.c</b>.

Many of the interfaces would be neater in C++, but I started out in C for some
reason.

The <b>INITIAL\_\*</b> lengths for data structures are excessively small
for testing purposes.
