// Evaluates the arithmetic expression in 's' using a top-down approach,
// storing the result in 'res'. Returns false without modifying 'res' if the
// expression is malformed. Catches all syntax errors.
//
// Supports integer arithmetic (to keep things simple), parentheses, and the
// operations +, -, *, /, and ** (exponentiation) with the usual precedence
// rules (and the "SAFE" rules for exponentation from
// http://macnauchtan.com/pub/precedence.html). Whitespace (isspace()) in
// expressions is ignored.
bool eval(const char *s, int *res);
