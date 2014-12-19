#include "common.h"
#include "arena_allocator.h"
#include "regex_compiler.h"

#pragma GCC diagnostic ignored "-Wswitch"

#define META  1
#define QUANT 2

// Used for returning to regex_compile() on syntax errors.
static jmp_buf err_jmp_buf;

// Metacharacters never stand for themselves (outside of brackets) and need to
// be escaped. '\0' is included for convenience when scanning for non-literals;
// patterns containing '\0' are not supported.

static const uint8_t char_properties[1 << CHAR_BIT] =
  { ['\0'] = META,
    ['.']  = META,
    ['*']  = META | QUANT,
    ['+']  = META | QUANT,
    ['?']  = META | QUANT,
    ['[']  = META,
    ['(']  = META,
    [')']  = META };

bool is_literal(char c) {
    return !(char_properties[(uc)c] & META);
}

bool is_quant(char c) {
    return char_properties[(uc)c] & QUANT;
}

static Regex_node *make_node(Regex_node_type type, Arena *arena) {
    Regex_node *node = arena_alloc(arena, sizeof *node);

    node->next = NULL;
    node->type = type;

    return node;
}

// Parses and returns a (possibly escaped) literal character. Works both
// outside and inside brackets.
static char parse_literal(const char **pat) {
    char res;

    if (**pat == '\\') {
        ++*pat; // Eat "\".
        if (**pat == '\0')
            // "\" at end of line.
            longjmp(err_jmp_buf, 1);
    }
    res = **pat;
    ++*pat;

    return res;
}

// parse_literals() helper. If the input starts with a non-quantified literal
// (a literal not followed by '?', '*', or '+'), move 'pat' past it and return
// true. Otherwise, return false without modifying 'pat'.
static bool next_nq_literal(const char **pat) {
    const char *tmp;

    if (!is_literal(**pat))
        return false;

    tmp = *pat;
    parse_literal(&tmp);
    if (is_quant(*tmp))
        return false;
    *pat = tmp;

    return true;
}

// Removes backslashes before escaped characters in 's' in-place.
static void deescape(char *s) {
    char *write_pos;

    for (write_pos = s;; ++s, ++write_pos) {
        if (*s == '\\')
            ++s;
        *write_pos = *s;
        if (*write_pos == '\0')
            return;
    }
}

// If the input has two or more non-quantified literals in a row, returns a
// SPAN node with the literals. Otherwise, returns a CHAR node with the initial
// literal. We must be careful to handle escaped literals correctly.
static Regex_node *parse_literals(const char **pat, Arena *arena) {
    const char *cur;
    char first_literal;
    Regex_node *node;

    if (!is_literal(**pat))
        longjmp(err_jmp_buf, 1);

    cur = *pat;

    first_literal = parse_literal(&cur);
    if (!next_nq_literal(&cur)) {
        // Single character. Use a CHAR node.
        node = make_node(CHAR, arena);
        node->c = first_literal;

        *pat = cur;

        return node;
    }

    // Many characters. Use a SPAN node.

    while (next_nq_literal(&cur));

    node = make_node(SPAN, arena);
    node->s = arena_strndup(arena, *pat, cur - *pat);
    deescape(node->s);

    *pat = cur;

    return node;
}

static Regex_node *parse_set(const char **pat, Arena *arena) {
    Regex_node *node = make_node(SET, arena);

    ++*pat; // Eat "[".

    // Create a look-up table for the set.
    node->set = arena_alloc(arena, sizeof(bool)*(1 << CHAR_BIT));
    for (unsigned i = 0; i < 1 << CHAR_BIT; ++i)
        node->set[i] = false;

    // An initial "]" is considered to be part of the set, so we always include
    // the first character.
    do {
        if (**pat == '\0')
            longjmp(err_jmp_buf, 1);
        node->set[(uc)parse_literal(pat)] = true;
    } while (**pat != ']');

    ++*pat; // Eat "]".

    return node;
}

// Modifies the node type according to the quantifier (if any).
static void quantify(const char **pat, Regex_node *node) {
    switch (**pat) {
    case '?': ++*pat; node->type += 1; break; // X -> X_ZERO_OR_ONE.
    case '*': ++*pat; node->type += 2; break; // X -> X_ZERO_OR_MORE.
    case '+': ++*pat; node->type += 3; break; // X -> X_ONE_OR_MORE.
    }
}

static Regex_node *parse_regex(const char **pat, Arena *arena);

static Regex_node *parse_node(const char **pat, Arena *arena) {
    Regex_node *node;

    switch (**pat) {
    case '.':
        ++*pat; // Eat ".".
        node = make_node(CHAR_ANY, arena);
        break;

    case '[':
        node = parse_set(pat, arena);
        break;

    case '(':
        ++*pat; // Eat "(".
        node = make_node(SUB, arena);
        node->sub = parse_regex(pat, arena);
        if (**pat != ')')
            longjmp(err_jmp_buf, 1);
        ++*pat; // Eat ")".
        break;

    default:
        node = parse_literals(pat, arena);
    }

    quantify(pat, node);

    return node;
}

static Regex_node *parse_regex(const char **pat, Arena *arena) {
    Regex_node *start, **cur;

    for (cur = &start; **pat != '\0' && **pat != ')'; cur = &(*cur)->next)
        *cur = parse_node(pat, arena);
    *cur = NULL;

    return start;
}

bool regex_compile(const char *pat, Regex *regex, Arena *arena) {
    if (setjmp(err_jmp_buf) == 1)
        return false;

    regex->start = parse_regex(&pat, arena);
    if (*pat != '\0')
        // Found extra trailing characters.
        return false;

    return true;
}
