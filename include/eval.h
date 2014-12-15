// Evaluates the arithmetic expression in 's' and stores the result in 'res'.
// Returns false without modifying 'res' if the expression is malformed.
//
// Supports parentheses and the operations +, -, *, /, and ** (exponentiation)
// with the usual precedence rules (and the "SAFE" rules for exponentation from
// http://macnauchtan.com/pub/precedence.html).
bool eval(const char *s, int *res);
