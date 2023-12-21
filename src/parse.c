#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "fsm_lexer.h"
#include "parse.h"

typedef struct {
    Ast* tree;
    char* stringRest;
} TOKEN_CONSUME;



Ast *ast_new(Ast ast) {
  Ast *ptr = malloc(sizeof(Ast));
  if (ptr) *ptr = ast;
  return ptr;
}

#define AST_NEW(tag, ...) \
  ast_new((Ast){tag, {.tag=(struct tag){__VA_ARGS__}}})

int isStringEq(char* string1, char* string2) {
  return strcmp(string1, string2) == 0;
}


// forward declaration

Ast* start(char* parseString);
TOKEN_CONSUME A(char* parseString), NA(char* parseString);

int isEmpty(Ast* ast){
    return ast->tag == Ast_empty;
}


// Start -> ( A ) | A IO A | PO A | L | ID

Ast* start(char* parseString) {
  TOKEN curTok = nextToken(parseString);
  int rest_empty = strlen(curTok.rest) == 0;
  // terminal trees
  if(curTok.tokenType == IDENTIFIER && rest_empty) {
    Ast *id = AST_NEW(Ast_identifier, *curTok.token);
    return id;
  }

  if(curTok.tokenType == LITERAL && rest_empty) {
     Ast *literal = AST_NEW(Ast_literal_false);
     if(*curTok.token == '0') literal = AST_NEW(Ast_literal_true);
     return literal;
  }
  
  // singular trees
  TOKEN_CONSUME subtree = A(curTok.rest);
  if(subtree.tree->tag == Ast_empty) {
    return AST_NEW(Ast_empty);
  }
  
  if(curTok.tokenType == PREFIX_OPERATOR) {
    Ast *not = AST_NEW(Ast_not, subtree.tree);
    return not;
  }

  if(curTok.tokenType == DELIMITER && *curTok.token == '(') {
    TOKEN nextDelim = nextToken(subtree.stringRest);

    if(nextDelim.tokenType != DELIMITER || *nextDelim.token != ')') return AST_NEW(Ast_empty);
    if(strlen(nextDelim.rest) != 0) return AST_NEW(Ast_empty);
    
    return subtree.tree;
  }

  // double tree 
  TOKEN_CONSUME leftSubtree = A(parseString);
  TOKEN operator = nextToken(leftSubtree.stringRest);
  TOKEN_CONSUME rightSubtree = A(operator.rest);

  if(leftSubtree.tree->tag == Ast_empty || rightSubtree.tree->tag == Ast_empty) {
    return AST_NEW(Ast_empty);
  }

  if(isStringEq(operator.token, "*")) {
    Ast *and = AST_NEW(Ast_and, leftSubtree.tree, rightSubtree.tree);
    return and;
  }

  if(isStringEq(operator.token, "+")) {
    Ast *or = AST_NEW(Ast_or, leftSubtree.tree, rightSubtree.tree);
    return or;
  }

  if(isStringEq(operator.token, "<=")) {
    Ast *limp = AST_NEW(Ast_li, leftSubtree.tree, rightSubtree.tree);
    return limp;
  }
  
  if(isStringEq(operator.token, "=>")) {
    Ast *rimp = AST_NEW(Ast_ri, leftSubtree.tree, rightSubtree.tree);
    return rimp;
  }

  if(isStringEq(operator.token, "<=>")) {
    Ast *eq = AST_NEW(Ast_eq, leftSubtree.tree, rightSubtree.tree);
    return eq;
  }

  return AST_NEW(Ast_empty);
}


// A -> ( A ) N.A | PO A N.A | L N.A | ID N.A | ( A )

TOKEN_CONSUME A(char* parseString) {
  TOKEN_CONSUME nyi = {AST_NEW(Ast_empty), ""};
  return nyi;
}


// N.A   -> IO A N.A | IO A

TOKEN_CONSUME NA(char* parseString) {
  TOKEN_CONSUME nyi = {AST_NEW(Ast_empty), ""};
  return nyi;
}



int main(void) {
  char* parseInput = "0";

  return EXIT_SUCCESS;
}
