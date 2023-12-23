//token.h 
#ifndef TOKEN_H
#define TOKEN_H
typedef enum {
    Invalid,
    // Items
    Identifier,
    BoolTrue, 
    BoolFalse,
    // Delimiters
    LeftParen,
    RightParen,
    // Operators
    Or, 
    And, 
    Not, 
    LeftImplication, 
    RightImplication,
    Equivalece,
} TOKEN_TYPE;

typedef struct {
  TOKEN_TYPE type;
  char* content;
} TOKEN;

void printToken(TOKEN* token);
char* tokenToStr(TOKEN* token);

#endif // !TOKEN_H
