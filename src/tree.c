#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

Ast *ast_new(Ast ast) {
  Ast *ptr = malloc(sizeof(Ast));
  if (ptr) *ptr = ast;
  return ptr;
}

int isAstEmpty(Ast* ast){
  return ast->tag == Ast_empty;
}

void ast_print(Ast* ptr){
  Ast ast = *ptr;

  printf("(");

  switch (ast.tag) {
    case Ast_and: {
      struct Ast_and data = ast.data.Ast_and;
      ast_print(data.left);
      printf("*");
      ast_print(data.right);
    }
    break;

    case Ast_identifier: {
      struct Ast_identifier data = ast.data.Ast_identifier;
      printf("%s", data.identifier);
    }
    break;

    case Ast_eq: {
      struct Ast_eq data = ast.data.Ast_eq;
      ast_print(data.left);
      printf("<=>");
      ast_print(data.right);
    }
    break;

    case Ast_li: {
      struct Ast_li data = ast.data.Ast_li;
      ast_print(data.left);
      printf("<=");
      ast_print(data.right);
    }
    break;


    case Ast_not: {
      struct Ast_not data = ast.data.Ast_not;
      printf("!");
      ast_print(data.right);
    }
    break;

    case Ast_or: {
      struct Ast_or data = ast.data.Ast_or;
      ast_print(data.left);
      printf("+");
      ast_print(data.right);
    }
    break;

    case Ast_ri: {
      struct Ast_ri data = ast.data.Ast_ri;
      ast_print(data.left);
      printf("=>");
      ast_print(data.right);
    }
    break;

    case Ast_literal_false: {
      struct Ast_literal_false data = ast.data.Ast_literal_false;
      printf("false");
    }
    break;

    case Ast_literal_true: {
      struct Ast_literal_true data = ast.data.Ast_literal_true;
      printf("true");
    }
    break;

    case Ast_empty: {
      struct Ast_empty data = ast.data.Ast_empty;
      printf("");
    }
    break;
  }

  printf(")");
}
