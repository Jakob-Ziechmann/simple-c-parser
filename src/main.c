#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"


Ast *ast_new(Ast ast) {
  Ast *ptr = malloc(sizeof(Ast));
  if (ptr) *ptr = ast;
  return ptr;
}

#define AST_NEW(tag, ...) \
  ast_new((Ast){tag, {.tag=(struct tag){__VA_ARGS__}}})


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
            printf("%c",data.identifier);
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







int main(void){
//test tree

/*Ast* term_ = ast_new((Ast){
    Ast_and, {
        .Ast_and = (struct Ast_and) {
            ast_new((Ast) {
                Ast_literal,
                {.Ast_literal = (struct Ast_literal) {'p'}}
            }),
            ast_new((Ast) {
                Ast_literal,
                {.Ast_literal = (struct Ast_literal) {'q'}}
            }),
        }
    }
}); */


    Ast* term = 
        AST_NEW(Ast_and, AST_NEW(Ast_identifier, 'p'), AST_NEW(Ast_identifier, 'q'));


    ast_print(term);
    printf("\n");
    
    return EXIT_SUCCESS;

}
    
