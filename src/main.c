#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "token.h"
#include "tree.h"
#include "lexer.h"


int main(void){

  LEXER* lexer = createLexer("!1");
  PARSER* parser = createParser(lexer);

  Ast* parseTree = parseExpression(parser);
  ast_print(parseTree);

  return EXIT_SUCCESS;
}
