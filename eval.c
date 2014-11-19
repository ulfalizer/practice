#include "common.h"
#include "eval.h"

static jmp_buf err_jmp_buf;

static int eval_sum(const char **s);

static int eval_num(const char **s) {
    int res = 0;
    if (!isdigit(**s))
        longjmp(err_jmp_buf, 1);
    while (isdigit(**s)) {
        res = 10*res + **s - '0';
        ++*s;
    }
    return res;
}

static int eval_exp(const char **s) {
    int base;

    while (isspace(**s))
        ++*s;

    switch (**s) {
    // Unary '+' and '-' operators.
    case '+': ++*s; base = eval_exp(s); break;
    case '-': ++*s; base = -eval_exp(s); break;
    case '(':
        {
        ++*s; // Eat "(".
        base = eval_sum(s);
        if (**s != ')')
            longjmp(err_jmp_buf, 1);
        ++*s; // Eat ")".
        break;
        }
    default:
        base = eval_num(s);
    }

    // Check if we have an exponent.

    while (isspace(**s))
        ++*s;

    if (**s == '*' && *(*s + 1) == '*') {
        *s += 2; // Eat "**"
        return pow(base, eval_exp(s));
    }
    return base;
}

static int eval_product(const char **s) {
    bool is_times = true;
    int product = 1;
    for (;;) {
        if (is_times)
            product *= eval_exp(s);
        else
            product /= eval_exp(s);

        switch (**s) {
        case '*': ++*s; is_times = true; continue;
        case '/': ++*s; is_times = false; continue;
        default: return product;
        }
    }
}

static int eval_sum(const char **s) {
    bool is_plus = true;
    int sum = 0;
    for (;;) {
        if (is_plus)
            sum += eval_product(s);
        else
            sum -= eval_product(s);

        switch (**s) {
        case '+': ++*s; is_plus = true; continue;
        case '-': ++*s; is_plus = false; continue;
        default: return sum;
        }
    }
}

bool eval(const char *s, int *res) {
    int tmp;
    if (setjmp(err_jmp_buf) == 1)
        return false;
    tmp = eval_sum(&s);
    if (*s == '\0') {
        *res = tmp;
        return true;
    }
    // Found extra trailing characters.
    return false;
}
