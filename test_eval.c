#include "common.h"
#include "eval.h"

void test_eval() {
    // Verifies that 'str' is not malformed and that it evaluates to 'res'.
    #define V(str, res)               \
      {                               \
          int result;                 \
          VERIFY(eval(str, &result)); \
          VERIFY(result == res);      \
      }

    // Verifies that 'str' is malformed.
    #define BAD(str)                   \
      {                                \
          int result = 567;            \
          VERIFY(!eval(str, &result)); \
          VERIFY(result == 567);       \
      }

    // One number.
    V("1", 1);
    V("123", 123);
    V("+123", 123);
    V("-123", -123);
    V("++123", 123);
    V("+-123", -123);
    V("-+123", -123);
    V("--123", 123);

    // Two numbers, sum/difference.
    V("101+102", 203);
    V("101-102", -1);
    V("101+-102", -1);
    V("101-+102", -1);
    V("-101+102", 1);
    V("-101-102", -203);

    // Two numbers, product/quotient.
    V("100*15", 1500);
    V("100/15", 6);
    V("100*-15", -1500);
    V("100/-15", -6);
    V("-100*15", -1500);
    V("-100/15", -6);

    // Sum/difference order of operations.
    V("2+3-4", 1);
    V("2-3+4", 3);
    V("2+3-4+5", 6);
    V("2-3+4-5", -2);

    // Product/quotient order of operations.
    V("2*2/1", 4);
    V("2/2*1", 1);
    V("2*2/2*2", 4);
    V("2/2*2/2", 1);

    // Sum/product precedence.
    V("1+4*2", 9);
    V("1*4+2", 6);
    V("2*4+1", 9);
    V("2+4*1", 6);

    // Parentheses.
    V("(123)", 123);
    V("((123))", 123);
    V("(((123)))", 123);
    V("(+123)", 123);
    V("(-123)", -123);
    V("(123)+(123)", 246);
    V("(2)*(3)", 6);
    V("(3+3)", 6);
    V("+(3+3)", 6);
    V("-(3+3)", -6);
    V("2*(3+3)", 12);
    V("(3+3)*2", 12);
    V("-(-1+1*3)*-(+(2+2)*3)", 24);

    // Exponentiation.
    V("1**0", 1);
    V("1**-1", 1);
    V("2**-1", 0);
    V("2**0", 1);
    V("2**1", 2);
    V("2**2", 4);
    V("2**3", 8);
    V("-2**2", -4);
    V("(-2)**2", 4);
    V("(-2)**3", -8);
    V("2**1**2", 2);
    V("(2**1)**2", 4);
    V("2**2*2", 8);
    V("2**(2*2)", 16);
    V("2**2+2", 6);
    V("2**(2+2)", 16);
    V("(2+2)**2", 16);
    V("(2+2)**(2+2)", 256);

    // Spacing.
    V("(+1+2+-4)*2**3/2", -4);
    V(" ( + 1 + 2 + - 4 ) * 2 ** 3 / 2 ", -4);
    V("  (  +  1  +  2  +  -  4  )  *  2  **  3  /  2  ", -4);

    // Malformed expressions.
    BAD("");
    BAD(" ");
    BAD("A");
    BAD("+");
    BAD("-");
    BAD("*");
    BAD("/");
    BAD("**");
    BAD("1A");
    BAD("1+");
    BAD("1-");
    BAD("1*");
    BAD("1/");
    BAD("1**");
    BAD("A1");
    BAD("*1");
    BAD("/1");
    BAD("**1");
    BAD("1A1");
    BAD("1+*1");
    BAD("1+/1");
    BAD("1-*1");
    BAD("1-/1");
    BAD("1*/1");
    BAD("1/*1");
    BAD("1+**1");
    BAD("1-**1");
    BAD("1***1");
    BAD("1/**1");
    BAD("1**/1");
    BAD("1/**1");
    BAD("1+1+");
    BAD("1-1-");
    BAD("1*1*");
    BAD("1/1/");
    BAD("(");
    BAD(")");
    BAD(")1");
    BAD("()");
    BAD("(A)");
    BAD("(+)");
    BAD("(-)");
    BAD("(*)");
    BAD("(/)");
    BAD("(**)");
    BAD("(*1)");
    BAD("(/1)");
    BAD("(**1)");
    BAD("(1+)");
    BAD("(1-)");
    BAD("(1*)");
    BAD("(1/)");
    BAD("(1**)");
    BAD("(1");
    BAD("1)");
    BAD("((1)");
    BAD("(1))");
    BAD("(1+1");
    BAD("1+1)");
    BAD("((1+1)");
    BAD("(1+1))");

    #undef V
    #undef BAD
}
