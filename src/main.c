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

    switch (ast.tag) {
        case Ast_and: {
            struct Ast_and data = ast.data.Ast_and;
            printf("&");
            ast_print(data.right);
            ast_print(data.left);
        }

        return;

        case Ast_literal: {
            struct Ast_literal data = ast.data.Ast_literal;
            printf("%c",data.literal);
        }
        return;
    }
}




char* str_cutoff(char* str, int cutindex){
    
    char* strcut = (char *)malloc(cutindex + 1); 
    for (int i = 0; i < cutindex; i++) {
        *(strcut + i) = *(str + i);
    }
    return strcut;
}



int main(void){
//test tree

Ast* term_ = ast_new((Ast){
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
});


    Ast* term = 
        AST_NEW(Ast_and, AST_NEW(Ast_literal, 'p'), AST_NEW(Ast_literal, 'q'));


    ast_print(term);
    printf("\n");
    
    return EXIT_SUCCESS;

}
    
