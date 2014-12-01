Small and simple implementations for various algorithms and data structures.
============================================================================

Function and macro documentation is in the headers.

Tests can be found in the <b>test\_*.c</b> files. The <b>test_tree.c</b> tests
use some variadic macro magic to do "visual" testing on trees (mostly for fun
as it's a bit cryptic, but it gives nicely readable test cases once you get
past the macro soup at least :)). Various other tricks can be found scattered
throughout.

Running:

    $ make
    $ ./test

The entry point is in <b>test.c</b>.

Many of the interfaces would be neater in C++, but I started out in C for some
reason.
