//parse.h
#include "tree.h"
#include "lexer.h"

#ifndef PARSE_H
#define PARSE_H

typedef struct {
  LEXER *lexer;

  TOKEN *currentToken;
  TOKEN *nextToken;
} PARSER;


PARSER* createParser(LEXER* lexer);
Ast* parseExpression(PARSER* parser, int precedence);

#endif // !PARSE_H
