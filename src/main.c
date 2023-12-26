#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "token.h"
#include "tree.h"
#include "lexer.h"


int main(void){

  LEXER* lexer = createLexer("vara + varb * varc => abc");
  PARSER* parser = createParser(lexer);

  Ast* parseTree = parseExpression(parser, 0);
  ast_print(parseTree);

  return EXIT_SUCCESS;
}
