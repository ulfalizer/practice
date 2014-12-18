#include "common.h"
#include "arena_allocator.h"
#include "vector.h"
#include "regex_compiler.h"
#include "string_builder.h"

#pragma GCC diagnostic ignored "-Wswitch"

#define A(...) string_append(s, ##__VA_ARGS__)

// Appends the characters from the (assumed) set node 'node' to 's', in sorted
// order.
static void set_repr(Regex_node *node, String *s) {
    for (int i = 0; i < 1 << CHAR_BIT; ++i)
        if (node->set[i])
            A("%c", i);
}

// Transforms a list of regex nodes into a string representation for testing,
// appending the result to 's'.
static void regex_repr(Regex_node *node, String *s) {
    for (; node != NULL; node = node->next) {
        switch (node->type) {
        case CHAR:              A("(C %c)", node->c); break;
        case CHAR_ZERO_OR_ONE:  A("(C? %c)", node->c); break;
        case CHAR_ZERO_OR_MORE: A("(C* %c)", node->c); break;
        case CHAR_ONE_OR_MORE:  A("(C+ %c)", node->c); break;

        case CHAR_ANY:              A("(.)"); break;
        case CHAR_ANY_ZERO_OR_ONE:  A("(.?)"); break;
        case CHAR_ANY_ZERO_OR_MORE: A("(.*)"); break;
        case CHAR_ANY_ONE_OR_MORE:  A("(.+)"); break;

        case SPAN:              A("(S %s)", node->s); break;
        case SPAN_ZERO_OR_ONE:  A("(S? %s)", node->s); break;
        case SPAN_ZERO_OR_MORE: A("(S* %s)", node->s); break;
        case SPAN_ONE_OR_MORE:  A("(S+ %s)", node->s); break;

        case SET:              A("(Z "); set_repr(node, s); A(")"); break;
        case SET_ZERO_OR_ONE:  A("(Z? "); set_repr(node, s); A(")"); break;
        case SET_ZERO_OR_MORE: A("(Z* "); set_repr(node, s); A(")"); break;
        case SET_ONE_OR_MORE:  A("(Z+ "); set_repr(node, s); A(")"); break;

        case SUB:              A("(P "); regex_repr(node->sub, s); A(")"); break;
        case SUB_ZERO_OR_ONE:  A("(P? "); regex_repr(node->sub, s); A(")"); break;
        case SUB_ZERO_OR_MORE: A("(P* "); regex_repr(node->sub, s); A(")"); break;
        case SUB_ONE_OR_MORE:  A("(P+ "); regex_repr(node->sub, s); A(")"); break;
        }
    }
}

#undef A

static void test_valid() {
    Arena arena;
    Arena_cursor arena_start;

    arena_init(&arena);
    arena_get_cursor(&arena, &arena_start);

    #define VERIFY_REPR(exp, repr)                      \
      {                                                 \
          Regex regex;                                  \
          String s;                                     \
                                                        \
          string_init(&s);                              \
                                                        \
          VERIFY(regex_compile(exp, &regex, &arena));   \
          regex_repr(regex.start, &s);                  \
          /* Rewind and reuse memory. */                \
          arena_set_cursor(&arena, &arena_start, true); \
          VERIFY(strcmp(string_get(&s), repr) == 0);    \
                                                        \
          string_free(&s);                              \
      }

    // Literals.
    VERIFY_REPR("", "");
    VERIFY_REPR("a", "(C a)");
    VERIFY_REPR("ab", "(S ab)");
    VERIFY_REPR("abc", "(S abc)");
    VERIFY_REPR("a b, c d!", "(S a b, c d!)");
    VERIFY_REPR("]", "(C ])");

    // Escaped literals.
    VERIFY_REPR("\\\\", "(C \\)");
    VERIFY_REPR("\\.", "(C .)");
    VERIFY_REPR("\\?", "(C ?)");
    VERIFY_REPR("\\*", "(C *)");
    VERIFY_REPR("\\+", "(C +)");
    VERIFY_REPR("\\(", "(C ()");
    VERIFY_REPR("\\)", "(C ))");
    VERIFY_REPR("\\[", "(C [)");
    VERIFY_REPR("\\]", "(C ])");
    VERIFY_REPR("\\a", "(C a)");
    VERIFY_REPR("a\\?\\*b\\+c", "(S a?*b+c)");
    VERIFY_REPR("\\\\\\\\", "(S \\\\)");

    // "Any" characters and literals.
    VERIFY_REPR(".", "(.)");
    VERIFY_REPR("..", "(.)(.)");
    VERIFY_REPR("a.", "(C a)(.)");
    VERIFY_REPR(".a", "(.)(C a)");
    VERIFY_REPR("a.b", "(C a)(.)(C b)");
    VERIFY_REPR("ab.", "(S ab)(.)");
    VERIFY_REPR(".ab", "(.)(S ab)");

    // Literals and quantifiers.
    VERIFY_REPR("a?", "(C? a)");
    VERIFY_REPR("a*", "(C* a)");
    VERIFY_REPR("a+", "(C+ a)");
    VERIFY_REPR("ab?", "(C a)(C? b)");
    VERIFY_REPR("ab*", "(C a)(C* b)");
    VERIFY_REPR("ab+", "(C a)(C+ b)");
    VERIFY_REPR("abc?", "(S ab)(C? c)");
    VERIFY_REPR("abc*", "(S ab)(C* c)");
    VERIFY_REPR("abc+", "(S ab)(C+ c)");
    VERIFY_REPR("a\\b+", "(C a)(C+ b)");
    VERIFY_REPR("\\a\\b\\c+", "(S ab)(C+ c)");

    // "Any" characters, literals, and quantifiers.
    VERIFY_REPR(".?", "(.?)");
    VERIFY_REPR(".*", "(.*)");
    VERIFY_REPR(".+", "(.+)");
    VERIFY_REPR("a.?", "(C a)(.?)");
    VERIFY_REPR("a.*", "(C a)(.*)");
    VERIFY_REPR("a.+", "(C a)(.+)");
    VERIFY_REPR("ab.?", "(S ab)(.?)");
    VERIFY_REPR("ab.*", "(S ab)(.*)");
    VERIFY_REPR("ab.+", "(S ab)(.+)");

    // Sets.
    VERIFY_REPR("[a]", "(Z a)");
    VERIFY_REPR("[.]", "(Z .)");
    VERIFY_REPR("[?]", "(Z ?)");
    VERIFY_REPR("[*]", "(Z *)");
    VERIFY_REPR("[+]", "(Z +)");
    VERIFY_REPR("[]]", "(Z ])");
    VERIFY_REPR("[]a]", "(Z ]a)");
    VERIFY_REPR("[[]", "(Z [)");
    VERIFY_REPR("[a]?", "(Z? a)");
    VERIFY_REPR("[a]*", "(Z* a)");
    VERIFY_REPR("[a]+", "(Z+ a)");
    VERIFY_REPR("[ba]", "(Z ab)");
    VERIFY_REPR("[ba]?", "(Z? ab)");
    VERIFY_REPR("[ba]*", "(Z* ab)");
    VERIFY_REPR("[ba]+", "(Z+ ab)");
    VERIFY_REPR("a[b]c", "(C a)(Z b)(C c)");
    VERIFY_REPR("ab[c]de", "(S ab)(Z c)(S de)");

    // Escaping in sets.
    VERIFY_REPR("[\\]]", "(Z ])");
    VERIFY_REPR("[a\\]]", "(Z ]a)");
    VERIFY_REPR("[\\\\]", "(Z \\)");
    VERIFY_REPR("[\\a]", "(Z a)");

    // Simple subexpressions.
    VERIFY_REPR("()", "(P )");
    VERIFY_REPR("(a)?", "(P? (C a))");
    VERIFY_REPR("(a)*", "(P* (C a))");
    VERIFY_REPR("(a)+", "(P+ (C a))");
    VERIFY_REPR("(.)?", "(P? (.))");
    VERIFY_REPR("(.)*", "(P* (.))");
    VERIFY_REPR("(.)+", "(P+ (.))");
    VERIFY_REPR("(ab)?", "(P? (S ab))");
    VERIFY_REPR("(ab)*", "(P* (S ab))");
    VERIFY_REPR("(ab)+", "(P+ (S ab))");
    VERIFY_REPR("([ab])?", "(P? (Z ab))");
    VERIFY_REPR("([ab])*", "(P* (Z ab))");
    VERIFY_REPR("([ab])+", "(P+ (Z ab))");

    // Subexpressions with quantifiers and many nodes.
    VERIFY_REPR("(a*)?", "(P? (C* a))");
    VERIFY_REPR("(.*)?", "(P? (.*))");
    VERIFY_REPR("([a]*)?", "(P? (Z* a))");
    VERIFY_REPR("(..)?", "(P? (.)(.))");
    VERIFY_REPR("(..)*", "(P* (.)(.))");
    VERIFY_REPR("(..)+", "(P+ (.)(.))");
    VERIFY_REPR("(a.)?", "(P? (C a)(.))");
    VERIFY_REPR("(a.)*", "(P* (C a)(.))");
    VERIFY_REPR("(a.)+", "(P+ (C a)(.))");
    VERIFY_REPR("(a[b])?", "(P? (C a)(Z b))");
    VERIFY_REPR("([a]b)?", "(P? (Z a)(C b))");
    VERIFY_REPR("(aa*)?", "(P? (C a)(C* a))");

    // Deeper nesting.
    VERIFY_REPR("a(b(c(d?)?e)*f)+g",
      "(C a)(P+ (C b)(P* (C c)(P? (C? d))(C e))(C f))(C g)");

    #undef VERIFY_REPR

    arena_free(&arena);
}

static void test_invalid() {
    Arena arena;
    Arena_cursor arena_start;

    arena_init(&arena);
    arena_get_cursor(&arena, &arena_start);

    #define VERIFY_BAD(exp)                             \
      {                                                 \
          Regex regex;                                  \
                                                        \
          VERIFY(!regex_compile(exp, &regex, &arena));  \
          arena_set_cursor(&arena, &arena_start, true); \
      }

    VERIFY_BAD("?");
    VERIFY_BAD("*");
    VERIFY_BAD("+");
    VERIFY_BAD("??");
    VERIFY_BAD("?*");
    VERIFY_BAD("?+");
    VERIFY_BAD("*?");
    VERIFY_BAD("**");
    VERIFY_BAD("*+");
    VERIFY_BAD("+?");
    VERIFY_BAD("+*");
    VERIFY_BAD("++");
    VERIFY_BAD("a??");
    VERIFY_BAD("a?*");
    VERIFY_BAD("a?+");
    VERIFY_BAD("a*?");
    VERIFY_BAD("a**");
    VERIFY_BAD("a*+");
    VERIFY_BAD("a+?");
    VERIFY_BAD("a+*");
    VERIFY_BAD("a++");

    VERIFY_BAD("\\");
    VERIFY_BAD("a\\");
    VERIFY_BAD("\\\\\\");

    VERIFY_BAD("[");
    VERIFY_BAD("a[");
    VERIFY_BAD("[a");
    VERIFY_BAD("[\\]");

    VERIFY_BAD("(");
    VERIFY_BAD("a(");
    VERIFY_BAD("(a");
    VERIFY_BAD(")");
    VERIFY_BAD("a)");
    VERIFY_BAD(")a");
    VERIFY_BAD("\\()");
    VERIFY_BAD("(\\)");

    #undef VERIFY_BAD

    arena_free(&arena);
}

void test_regex() {
    test_valid();
    test_invalid();
}
