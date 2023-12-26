//lexer.h 
#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
  char* input;
  int position;
} LEXER;

LEXER* createLexer(char* input);
TOKEN* nextToken(LEXER* lexer);
int inputIsRead(LEXER* lexer);

#endif // !LEXER_H

