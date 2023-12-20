#include <stdio.h>
#include <stdlib.h>



typedef enum {
  IDENTIFIER,
  LITERAL,
  DELIMITER,
  PREFIX_OPERATOR,
  INFIX_OPRATOR,
  ERROR,
} TOKEN_TYPE;

typedef struct {
  TOKEN_TYPE tokenType;
  char* token;
} TOKEN;

// forward declaration

int A(void), NA(void), start(void);

//variable token

TOKEN_TYPE x = IDENTIFIER;
char* y = "123";



TOKEN token = {x, y};

// Start -> ( A ) | A IO A | PO A | L | ID

int start(void) {
    if (token.tokenType == DELIMITER) {
        if (A()) {
            if (token.tokenType == DELIMITER){
                return 1;
            }
        }
    }
    else if (A()){
        if (token.tokenType == INFIX_OPRATOR){
            if (A()){
                return 1;
            }
        }
    }
    else if (token.tokenType == PREFIX_OPERATOR) {
        if (A()){
            return 1;
        }
    }
    else if (token.tokenType == LITERAL){
        return 1;
    } 
    else if (token.tokenType == IDENTIFIER) {
        return 1;
    }
    else {
        return 0;
    } 
}


// A -> ( A ) N.A | PO A N.A | L N.A | ID N.A | ( A )

int A(void) {
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

int NA(void) {
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
    return EXIT_SUCCESS;
}