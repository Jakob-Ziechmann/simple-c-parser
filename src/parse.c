#include "parse.h"
#include "fsm_lexer.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef struct {
  Ast *tree;
  char *stringRest;
} TOKEN_CONSUME;

int isStringEq(char *string1, char *string2) {
  return strcmp(string1, string2) == 0;
}

// forward declaration

Ast *start(char *parseString);
TOKEN_CONSUME A(char *parseString), NA(char *parseString);

Ast *parseString(char *string) { return start(string); }

// Start -> ( A ) | A IO A | PO A | L | ID

Ast *start(char *parseString) {
  TOKEN curTok = nextToken(parseString);
  int rest_empty = (strlen(curTok.rest) == 0);

  // terminal trees
  if (curTok.tokenType == IDENTIFIER && rest_empty) {
    Ast *id = AST_NEW(Ast_identifier, curTok.token);
    return id;
  }

  if (curTok.tokenType == LITERAL && rest_empty) {
    Ast *literal = AST_NEW(Ast_literal_false);
    if (*curTok.token == '1')
      literal = AST_NEW(Ast_literal_true);
    return literal;
  }

  // singular trees
  TOKEN_CONSUME subtree = A(curTok.rest);
  if (subtree.tree->tag == Ast_empty) {
    return AST_NEW(Ast_empty);
  }

  if (curTok.tokenType == PREFIX_OPERATOR) {
    Ast *not = AST_NEW(Ast_not, subtree.tree);
    return not ;
  }

  if (curTok.tokenType == DELIMITER && *curTok.token == '(') {
    TOKEN nextDelim = nextToken(subtree.stringRest);

    if (nextDelim.tokenType != DELIMITER)
      return AST_NEW(Ast_empty);
    if (*nextDelim.token != ')')
      return AST_NEW(Ast_empty);
    if (strlen(nextDelim.rest) != 0)
      return AST_NEW(Ast_empty);

    return subtree.tree;
  }

  // double tree
  TOKEN_CONSUME leftSubtree = A(parseString);
  TOKEN operator= nextToken(leftSubtree.stringRest);
  TOKEN_CONSUME rightSubtree = A(operator.rest);

  if (leftSubtree.tree->tag == Ast_empty ||
      rightSubtree.tree->tag == Ast_empty) {
    return AST_NEW(Ast_empty);
  }

  if (isStringEq(operator.token, "*")) {
    Ast *and = AST_NEW(Ast_and, leftSubtree.tree, rightSubtree.tree);
    return and;
  }

  if (isStringEq(operator.token, "+")) {
    Ast * or = AST_NEW(Ast_or, leftSubtree.tree, rightSubtree.tree);
    return or ;
  }

  if (isStringEq(operator.token, "<=")) {
    Ast *limp = AST_NEW(Ast_li, leftSubtree.tree, rightSubtree.tree);
    return limp;
  }

  if (isStringEq(operator.token, "=>")) {
    Ast *rimp = AST_NEW(Ast_ri, leftSubtree.tree, rightSubtree.tree);
    return rimp;
  }

  if (isStringEq(operator.token, "<=>")) {
    Ast *eq = AST_NEW(Ast_eq, leftSubtree.tree, rightSubtree.tree);
    return eq;
  }

  return AST_NEW(Ast_empty);
}

// A -> ( A ) N.A | PO A N.A | L N.A | ID N.A

TOKEN_CONSUME A(char *parseString) {
  TOKEN curTok = nextToken(parseString);
  Ast *subtree = AST_NEW(Ast_empty);
  char *rest = "";
  TOKEN_CONSUME error = {subtree, parseString};

  if (curTok.tokenType == IDENTIFIER) {
    subtree = AST_NEW(Ast_identifier, curTok.token);
    rest = curTok.rest;
  }

  if (curTok.tokenType == LITERAL) {
    if (*curTok.token == '0') {
      subtree = AST_NEW(Ast_literal_false);
    } else {
      subtree = AST_NEW(Ast_literal_true);
    }
    rest = curTok.rest;
  }

  if (curTok.tokenType == PREFIX_OPERATOR) {
    TOKEN_CONSUME aResSubtree = A(curTok.rest);
    subtree = AST_NEW(Ast_not, aResSubtree.tree);
    rest = aResSubtree.stringRest;
  }

  if (curTok.tokenType == DELIMITER && *curTok.token == '(') {
    TOKEN_CONSUME aResSubtree = A(curTok.rest);
    TOKEN nextDelim = nextToken(aResSubtree.stringRest);

    if (nextDelim.tokenType != DELIMITER || *nextDelim.token != ')')
      return error;
    // if(strlen(nextDelim.rest) != 0) return error;

    subtree = aResSubtree.tree;
    rest = nextDelim.rest;
  }

  TOKEN_CONSUME naResult = NA(rest);
  if (naResult.tree->tag == Ast_empty) {
    TOKEN_CONSUME result = {subtree, rest};
    return result;
  }

  Ast *natree = naResult.tree;
  switch (natree->tag) {
  case (Ast_and):
    natree->data.Ast_and.left = subtree;
    break;

  case (Ast_or):
    natree->data.Ast_or.left = subtree;
    break;

  case (Ast_li):
    natree->data.Ast_li.left = subtree;
    break;

  case (Ast_ri):
    natree->data.Ast_ri.left = subtree;
    break;

  case (Ast_eq):
    natree->data.Ast_eq.left = subtree;
    break;

  default:
    break;
  }
  return naResult;
}

// N.A   -> IO A N.A | epsilon

TOKEN_CONSUME NA(char *parseString) {

  TOKEN curTok = nextToken(parseString);
  Ast *subtree = AST_NEW(Ast_empty);
  char *rest = "";
  TOKEN_CONSUME error = {subtree, parseString};
  TOKEN_CONSUME result = {AST_NEW(Ast_empty)};

  if (strlen(parseString) == 0) {
    return error;
  }

  Ast *emptyTree = AST_NEW(Ast_empty);

  if (curTok.tokenType == INFIX_OPRATOR) {
    TOKEN_CONSUME subtreeA = A(curTok.rest);
    rest = subtreeA.stringRest;

    if (strcmp(curTok.token, "<=>") == 0) {
      subtree = AST_NEW(Ast_eq, emptyTree, subtreeA.tree);
    }
    if (strcmp(curTok.token, "<=") == 0) {
      subtree = AST_NEW(Ast_li, emptyTree, subtreeA.tree);
    }
    if (strcmp(curTok.token, "=>") == 0) {
      subtree = AST_NEW(Ast_ri, emptyTree, subtreeA.tree);
    }
    if (strcmp(curTok.token, "+") == 0) {
      subtree = AST_NEW(Ast_or, emptyTree, subtreeA.tree);
    }
    if (strcmp(curTok.token, "*") == 0) {
      subtree = AST_NEW(Ast_and, emptyTree, subtreeA.tree);
    } else {
      subtree = AST_NEW(Ast_empty);
    }
  }

  TOKEN_CONSUME naResult = NA(rest);
  if (naResult.tree->tag == Ast_empty) {
    TOKEN_CONSUME result = {subtree, rest};
    return result;
  }

  Ast *natree = naResult.tree;
  switch (natree->tag) {
  case (Ast_and):
    natree->data.Ast_and.left = subtree;
    break;

  case (Ast_or):
    natree->data.Ast_or.left = subtree;
    break;

  case (Ast_li):
    natree->data.Ast_li.left = subtree;
    break;

  case (Ast_ri):
    natree->data.Ast_ri.left = subtree;
    break;

  case (Ast_eq):
    natree->data.Ast_eq.left = subtree;
    break;

  default:
    break;
  }

  return naResult;
}

// int main(void) {
//   char* parseInput = "0";
//
//   return EXIT_SUCCESS;
// }
