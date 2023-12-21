//parse.h
#ifndef PARSE_H
#define PARSE_H

typedef struct Ast Ast; // Forward reference

struct Ast {
  enum {
    Ast_literal_true,
    Ast_literal_false,
    Ast_identifier,
    Ast_not,
    Ast_and,
    Ast_or,
    Ast_ri,
    Ast_li,
    Ast_eq,
    Ast_empty,
  } tag;
  union {
    struct Ast_literal_true {} Ast_literal_true;
    struct Ast_literal_false {} Ast_literal_false;
    struct Ast_identifier { char identifier;} Ast_identifier;
    struct Ast_not {Ast *right;} Ast_not;
    struct Ast_and {Ast *left; Ast *right;} Ast_and;
    struct Ast_or {Ast *left; Ast *right;} Ast_or;
    struct Ast_li {Ast *left; Ast *right;} Ast_li;
    struct Ast_ri {Ast *left; Ast *right;} Ast_ri;
    struct Ast_eq {Ast *left; Ast *right;} Ast_eq;
    struct Ast_empty {} Ast_empty;
  } data;
};

#endif // !PARSE_H
