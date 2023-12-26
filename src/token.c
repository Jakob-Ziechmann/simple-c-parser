#include "token.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


TOKEN* createToken(TOKEN_TYPE type, char* content) {
  TOKEN newToken = {type, content};
  TOKEN *ptr = malloc(sizeof(newToken));
  *ptr = newToken;
  return ptr;
}

int isValid(TOKEN *token) {
  return token->type != Invalid;
}

char* tokenToStr(TOKEN *token) {
  if(token == NULL) return "NULL TOKEN";
  switch (token->type) {
    case(Identifier): {
      char* desc = "Identifier: ";
      char* name = token->content;
      return cat(desc, name);
    }
    case(BoolTrue): return "Bool: True";
    case(BoolFalse): return "Bool: False";
    case(LeftParen): return "Delimiter: (";
    case(RightParen): return "Delimiter: )";
    case(Or): return "Operator: Or +";
    case(And): return "Operator: And *";
    case(Not): return "Operator: Not !";
    case(LeftImplication): return "Operator: Implication <=";
    case(RightImplication): return "Operator: Implication =>";
    case(Equivalece): return "Operator: Equivalece <=>";
    default: return "Invalid Token";
  }
}

void printToken(TOKEN *token) {
  printf("%s\n", tokenToStr(token));
}
