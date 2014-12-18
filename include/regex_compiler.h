// Simple NFA regex compiler. Compiles regexes to lists of nodes that should be
// handy for matching. Performs trivial simplification by using dedicated node
// types for common patterns and merging spans of multiple literal characters
// into single SPAN nodes.
//
// - Supported quantifiers: '?', '*', and '+'.
//
// - Supports '.' (match any).
//
// - Supports sets ("[]") and subexpressions ("()"). The first character listed
//   in a set is always included in the set, so "[]]" is a set containing just
//   ']'. ']' is treated as a literal character outside of sets, so a ']' by
//   itself is a valid regular expression.
//
// - Supports backslash-escaping of metacharacters (and ordinary characters) to
//   turn them into literal characters, both outside of and inside sets.
//
// Implementing a matcher should reveal further refinments. :)

typedef enum Regex_node_type {
    // Can be interpreted as the least significant two bits holding the
    // quantifier and the higher bits the type.
    CHAR = 0, CHAR_ZERO_OR_ONE    , CHAR_ZERO_OR_MORE    , CHAR_ONE_OR_MORE    ,
    CHAR_ANY, CHAR_ANY_ZERO_OR_ONE, CHAR_ANY_ZERO_OR_MORE, CHAR_ANY_ONE_OR_MORE,
    SPAN    , SPAN_ZERO_OR_ONE    , SPAN_ZERO_OR_MORE    , SPAN_ONE_OR_MORE    ,
    SET     , SET_ZERO_OR_ONE     , SET_ZERO_OR_MORE     , SET_ONE_OR_MORE     ,
    SUB     , SUB_ZERO_OR_ONE     , SUB_ZERO_OR_MORE     , SUB_ONE_OR_MORE
} Regex_node_type;

typedef struct Regex_node {
    // A more compact representation without 'next' pointers would be possible.
    struct Regex_node *next;
    Regex_node_type type;
    union {
        char c; // CHAR.
        char *s; // SPAN.
        bool *set; // SET.
        struct Regex_node *sub; // SUB.
    };
} Regex_node;

typedef struct Regex {
    Regex_node *start;
} Regex;

// Compiles the regex and stores a representation in 'regex'. Memory is
// allocated from 'arena', which should already be initialized. Returns
// false if the regex is malformed.
//
// In more practical code we could pass an allocator_t or just hardcode an
// allocator internally (though an arena allocator is still handy for freeing
// memory on errors, and fast for many small allocations).
bool regex_compile(const char *pattern, Regex *regex, Arena *arena);
