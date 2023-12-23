#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"
#include "tree.h"


int main(void){

  // Ast* term = 
  //   AST_NEW(Ast_and, AST_NEW(Ast_identifier, "y"), AST_NEW(Ast_identifier, "p"));
  // ast_print(term);

  char* test = "(a+b)=>c";
  Ast* parsedString = parseString(test);


  ast_print(parsedString);
  printf("\n");
  
  return EXIT_SUCCESS;
}
