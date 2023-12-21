//fsm_lexer.h 
#ifndef FSM_LEXER_H
#define FSM_LEXER_H

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
  char* rest;
} TOKEN;

TOKEN nextToken(char* string);
char* tokenToStr(TOKEN token);
char* tokenTypeToStr(TOKEN_TYPE string);

#endif // !FSM_LEXER_H
