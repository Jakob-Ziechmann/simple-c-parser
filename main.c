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
        struct Ast_literal {int literal;} Ast_literal;
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
  ast_new((AST){tag, {.tag=(struct tag){__VA_ARGS__}}})

Ast* test(void) {
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
}


void ast_print(Ast* ptr){
    Ast ast = *ptr;

    switch (ast.tag) {
        case Ast_and: {
            struct Ast_and data = ast.data.Ast_and;
            printf("&");
        }

        return;

        case Ast_literal: {
            struct Ast_literal data = ast.data.Ast_literal;
            printf("%c",data.literal);
        }
        return;
    }
}





enum tokenizer {start = 0, az = 1, singleop = 2, lieq = 3, lieq2 = 4, ri = 5, rieq = 6, literal = 7, trap = 8};

enum tokenizer state = start;

int isAcceptingState(enum tokenizer state){
    switch (state) {
        case start: return 0;
        case lieq: return 0;
        case ri: return 0;
        default: return 1;
    }
}

int findMaxToken(enum tokenizer state, char* x){
    char* currunt = x;
    char* next = x + 1;
    enum tokenizer nextState = trap;
    
    switch (state) {
        case start: 
            if (isalpha(*currunt)) nextState = az;
            if (*currunt == '(' | *currunt == ')' | *currunt == '|' | *currunt == '&' | *currunt == '!') nextState = singleop;
            if (*currunt == '<') nextState = lieq;
            if (*currunt == '=') nextState = ri;
            break;

        case az:
            if (isalpha(*currunt)) nextState = az;
            break;

        case lieq:
            if (*currunt == '=') nextState = lieq2;
            break;

        case lieq2:
            if (*currunt == '>') nextState = rieq;
            break;

        case ri:
            if (*currunt == '>') nextState = rieq;
            break;

        default: 
            break;
    }


    printf("%d\n", (int)nextState);
    if (nextState == trap) return 0;

    int maxToken = findMaxToken(nextState, next);

    if (maxToken == 0 && !(isAcceptingState(nextState))) {return 0;}
    return maxToken + 1;

}

char* str_cutoff(char* str, int cutindex){
    
    char* strcut = (char *)malloc(cutindex + 1); 
    for (int i = 0; i < cutindex; i++) {
        *(strcut + i) = *(str + i);
    }
    return strcut;
}





/* char* nextToken(char* x){
    int toklen = findMaxToken(start, x);
    char* y = (char *)malloc(strlen(x) + 1);
    return str_cutoff(x, y, toklen);
} */

int main(void){
    char* x = "hallo(hallo)";
    char* y = (char *)malloc(strlen(x) + 1);
    printf("%d\n", findMaxToken(start, x));
    //printf("%s\n", nextToken(x));
    char *d = str_cutoff(x, 3);
    printf("%s\n", d);

    Ast* z = test;
    ast_print(z);
    //printf("%c\n", *y);
    //printf("%s\n", y);
    
    return EXIT_SUCCESS;

}
    