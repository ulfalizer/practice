Small and simple implementations of various algorithms and data structures.
===========================================================================


* The [include](include) directory contains headers with (extensive) function and macro documentation.
* The [src](src) directory contains the implementations.
* The [tests](tests) directory contains unit tests.

The code makes use of C99/11 features, including variadic macros, <b>\<stdbool.h\></b>,
and <b>alignas()</b> (for the arena allocator).

Running:

    $ make
    $ ./test

The entry point is in [tests/test.c](tests/test.c)</b>.

Many of the interfaces would be neater in C++, but I started out in C for some
reason.

The initial lengths for data structures are excessively small
for testing purposes.
