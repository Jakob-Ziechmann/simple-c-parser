//tree.h 
#ifndef TREE_H
#define TREE_H

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
    struct Ast_identifier { char *identifier;} Ast_identifier;
    struct Ast_not {Ast *right;} Ast_not;
    struct Ast_and {Ast *left; Ast *right;} Ast_and;
    struct Ast_or {Ast *left; Ast *right;} Ast_or;
    struct Ast_li {Ast *left; Ast *right;} Ast_li;
    struct Ast_ri {Ast *left; Ast *right;} Ast_ri;
    struct Ast_eq {Ast *left; Ast *right;} Ast_eq;
    struct Ast_empty {} Ast_empty;
  } data;
};

Ast *ast_new(Ast ast);
int isAstEmpty(Ast* ast);
void ast_print(Ast* ast);

#define AST_NEW(tag, ...) \
  ast_new((Ast){tag, {.tag=(struct tag){__VA_ARGS__}}})

#endif // !TREE_H


