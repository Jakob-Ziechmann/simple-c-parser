#include <stdio.h>
#include <stdlib.h>
#include "fsm_lexer.h"


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
        Ast_empty
    } tag;
    union {
        struct Ast_literal {char literal;} Ast_literal; //mistake
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


typedef struct {
    Ast* tree;
    char* stringRest;
} tokenConsume;



Ast *ast_new(Ast ast) {
  Ast *ptr = malloc(sizeof(Ast));
  if (ptr) *ptr = ast;
  return ptr;
}

#define AST_NEW(tag, ...) \
  ast_new((Ast){tag, {.tag=(struct tag){__VA_ARGS__}}})



// forward declaration

tokenConsume A(), NA(), start();

//variable token

TOKEN_TYPE x = IDENTIFIER;
char* y = "123";


TOKEN token;


int isEmpty(Ast* ast){
    return ast->tag == Ast_empty;
}


// Start -> ( A ) | A IO A | PO A | L | ID

tokenConsume start() {
    if (token.tokenType == DELIMITER) {
        if (!(isEmpty(A().tree))) {
            if (token.tokenType == DELIMITER){
                return ();
            }
        }
    }
    else if (A()){
        if (token.tokenType == INFIX_OPRATOR){
            if (A()){
                return x;
            }
        }
    }
    else if (token.tokenType == PREFIX_OPERATOR) {
        if (A()){
            return x;
        }
    }
    else if (token.tokenType == LITERAL){
        return x;
    } 
    else if (token.tokenType == IDENTIFIER) {
        return x;
    }
    else {
        char* error = "Error";
        Ast* empty = AST_NEW(Ast_empty, );
        x = (empty, error);   //return empty tree
        return x;
    } 
}


// A -> ( A ) N.A | PO A N.A | L N.A | ID N.A | ( A )

tokenConsume A(void) {
    if (token.tokenType == DELIMITER){
        if (A()){
            if (token.tokenType == DELIMITER){
                if (NA()) {
                    return 1;
                }
            }
        }
    }
    else if (token.tokenType == PREFIX_OPERATOR){
        if (A()){
            if (NA()) {
                return 1;
            }
        }
    }
    else if (token.tokenType == LITERAL) {
        if (NA()) {
            return 1;
        }
    }
    else if (token.tokenType == IDENTIFIER) {
        if (NA()) {
            return 1;
        }
    }
    else if (token.tokenType == DELIMITER) {
        if (A()) {
            if (token.tokenType == DELIMITER){
                return 1;
            }
        }
    }
    else return 0; 

}


// N.A   -> IO A N.A | IO A

tokenConsume NA(void) {
    if (token.tokenType == INFIX_OPRATOR) {
        if (A()) {
            if (NA()) {
                return 1;
            }
        }
    }
    else if (token.tokenType == INFIX_OPRATOR) {
        if (A()) {
            return 1;
        }
    }
    else return 0;
}



int main(void) {
    TOKEN token = {x, y};
    return EXIT_SUCCESS;
}