#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


typedef struct Ast Ast; // Forward reference

struct Ast {
    enum {
        Ast_literal,
        Ast_identifier,
        Ast_not,
        Ast_and,
        Ast_or,
        Ast_ri,
        Ast_li,
        Ast_eq,
    } tag;
    union {
        struct Ast_literal {char literal;} Ast_literal;
        struct Ast_identifier { char identifier;} Ast_identifier;
        struct Ast_not {Ast *right;} Ast_not;
        struct Ast_and {Ast *left; Ast *right;} Ast_and;
        struct Ast_or {Ast *left; Ast *right;} Ast_or;
        struct Ast_li {Ast *left; Ast *right;} Ast_li;
        struct Ast_ri {Ast *left; Ast *right;} Ast_ri;
        struct Ast_eq {Ast *left; Ast *right;} Ast_eq;

  } data;
};


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
    