#include "lexer.h"
#include "token.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

LEXER* createLexer(char* input) {
  LEXER newLexer = {input, 0};
  LEXER *ptr = malloc(sizeof(newLexer));
  *ptr = newLexer;
  return ptr;
}

char firstChar(LEXER* lexer) {
  if(strlen(lexer->input) > lexer->position) {
    return *(lexer->input + lexer->position);
  }
  return '\0';
}

int inputIsRead(LEXER* lexer) {
  return *(lexer->input + lexer->position) == '\0';
}

void advanceLexer(LEXER* lexer) {
  if(strlen(lexer->input) > lexer->position) {
    lexer->position++;
  }
}

void skipWhiteSpace(LEXER* lexer) {
  while (firstChar(lexer) == ' ') {
    advanceLexer(lexer);
  }
}

int peakAhead(LEXER *lexer, char* string) {
  int contains = 1;
  int* pos = &lexer->position;
  for(int i = 0; i < (strlen(lexer->input) - *pos) && i < strlen(string); i++){
    contains *= *(lexer->input + i + *pos) == *(string + i);
  }
  return contains;
}

int isIdentifier(char token) {
  return isalpha(token);
}

char* readIdentifier(LEXER *lexer) {
  char* out = "";
  while(isIdentifier(firstChar(lexer))) {
    char firstc[] = {firstChar(lexer), '\0'};
    out = cat(out, firstc);
    advanceLexer(lexer);
  }
  return out;
}

TOKEN nextToken(LEXER* lexer) {
  skipWhiteSpace(lexer);
  TOKEN invalid = {Invalid, ""};
  if(inputIsRead(lexer)) return invalid;
  

  switch(firstChar(lexer)) {
    case('('): 
      advanceLexer(lexer);
      TOKEN leftpar = {LeftParen, ""};
      return leftpar;

    case(')'): 
      advanceLexer(lexer);
      TOKEN rightpar = {RightParen, ""};
      return rightpar;

    case('+'): 
      advanceLexer(lexer);
      TOKEN or = {Or, ""};
      return or;

    case('*'): 
      advanceLexer(lexer);
      TOKEN and = {And, ""};
      return and;

    case('!'): 
      advanceLexer(lexer);
      TOKEN not = {Not, ""};
      return not;

    case('0'):
      advanceLexer(lexer);
      TOKEN bfalse = {BoolFalse, ""};
      return bfalse;

    case('1'):
      advanceLexer(lexer);
      TOKEN btrue = {BoolTrue, ""};
      return btrue;

    default:;
  }

  if(peakAhead(lexer, "<=>")) {
    advanceLexer(lexer);
    advanceLexer(lexer);
    advanceLexer(lexer);
    TOKEN eq = {Equivalece, ""};
    return eq;
  }

  if(peakAhead(lexer, "<=")) {
    advanceLexer(lexer);
    advanceLexer(lexer);
    TOKEN limp = {LeftImplication, ""};
    return limp;
  }

  if(peakAhead(lexer, "=>")) {
    advanceLexer(lexer);
    advanceLexer(lexer);
    TOKEN rimp = {RightImplication, ""};
    return rimp;
  }


  if(isIdentifier(firstChar(lexer))) {
    char* name = readIdentifier(lexer);
    TOKEN token = {Identifier, name};
    return token;
  }

  return invalid;
}

