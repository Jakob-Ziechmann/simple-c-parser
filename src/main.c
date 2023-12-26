#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "token.h"
#include "tree.h"
#include "lexer.h"


int main(void){

  ast_print(parse("(ab => c) * (0 => ab + d)"));

  return EXIT_SUCCESS;
}
