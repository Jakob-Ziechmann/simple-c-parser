#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
// #include "parse.h"
#include "token.h"
#include "tree.h"
#include "lexer.h"


int main(void){

  LEXER *lexer = createLexer("abc + => ( hallo )");


  while (!inputIsRead(lexer)) {
    TOKEN token = nextToken(lexer);
    printToken(&token);
  }

  return EXIT_SUCCESS;
}
