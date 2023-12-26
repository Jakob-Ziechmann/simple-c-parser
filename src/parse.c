#include "parse.h"
#include "token.h"
#include "tree.h"
#include "lexer.h"
#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>

void consumeToken(PARSER* parser);

PARSER* createParser(LEXER* lexer) {
  PARSER newParser = {lexer, NULL, NULL};

  PARSER *ptr = malloc(sizeof(newParser));
  *ptr = newParser;

  consumeToken(ptr);
  consumeToken(ptr);

  return ptr;
}

Ast* parseIdentifier(PARSER* parser);
Ast* parseBoolean(PARSER* parser);
Ast* parseNot(PARSER* parser);
Ast* parseGroup(PARSER* parser);

void consumeToken(PARSER* parser) {
  parser->currentToken = parser->nextToken;
  parser->nextToken = nextToken(parser->lexer);
}


int checkParsePrefix(TOKEN* token) {
  switch (token->type) {
    case(Identifier):
    case(BoolTrue):
    case(BoolFalse):
    case(Not):
    case(LeftParen):

    break;
    default: return 0;
  }
  return 1;
}

int checkParseInfix(TOKEN* token) {
  switch (token->type) {
    case(And):
    case(Or):
    case(LeftImplication):
    case(RightImplication):
    case(Equivalece):

    break;
    default: return 0;
  }
  return 1;
}

Ast* parsePrefix(PARSER* parser) {
  switch (parser->currentToken->type) {
    case(Identifier): return parseIdentifier(parser);
    case(BoolTrue):
    case(BoolFalse): return parseBoolean(parser);
    case(Not): return parseNot(parser);
    case(LeftParen): return parseGroup(parser);

    default:;
  }
  return AST_NEW(Ast_empty);
}

Ast* parseIdentifier(PARSER* parser) {
  TOKEN *idToken = parser->currentToken;
  if(idToken->type != Identifier) return AST_NEW(Ast_empty);

  Ast* identifier = AST_NEW(Ast_identifier, idToken->content);
  consumeToken(parser);
  return identifier;
}

Ast* parseBoolean(PARSER* parser) {
  TOKEN* boolToken = parser->currentToken;

  Ast* boolTree;
  switch (boolToken->type) {
    case(BoolTrue):
      boolTree = AST_NEW(Ast_literal_true);
      break;
    case(BoolFalse): 
      boolTree = AST_NEW(Ast_literal_false);
      break;
    default: return AST_NEW(Ast_empty);
  }

  consumeToken(parser);
  return boolTree;
}

Ast* parseNot(PARSER* parser) {
  TOKEN notToken = *parser->currentToken;
  if(notToken.type != Not) return AST_NEW(Ast_empty);

  consumeToken(parser);
  Ast* rightTree = parseExpression(parser, 0);
  return AST_NEW(Ast_not, rightTree);
}

Ast* parseGroup(PARSER* parser) {
  consumeToken(parser);

  Ast* expression = parseExpression(parser, 0);

  if(parser->currentToken->type != RightParen) {
    return AST_NEW(Ast_empty);
  }

  consumeToken(parser);

  return expression;

}

int precedence(TOKEN* token) {
  switch (token->type) {
    case(Not): return 4;
    case(And): return 3;
    case(Or): return 2;
    case(LeftImplication): return 1;
    case(RightImplication): return 1;
    case(Equivalece): return 1;
    default: return 0;
  }
}

Ast* parseInfix(PARSER* parser, Ast* left) {
  TOKEN operator = *parser->currentToken;
  int prec = precedence(&operator);

  consumeToken(parser);

  Ast* right = parseExpression(parser, prec);

  switch (operator.type) {
    case(And): return AST_NEW(Ast_and, left, right);
    case(Or): return AST_NEW(Ast_or, left, right);
    case(LeftImplication): return AST_NEW(Ast_li, left, right);
    case(RightImplication): return AST_NEW(Ast_ri, left, right);
    case(Equivalece): return AST_NEW(Ast_eq, left, right);
    default: return AST_NEW(Ast_empty);
  }
}

//Parser
Ast* parseExpression(PARSER* parser, int prec) {
  int validPrefix = checkParsePrefix(parser->currentToken);

  if(!validPrefix) {
    return AST_NEW(Ast_empty);
  }

  Ast* leftExpression = parsePrefix(parser);


  while(parser->nextToken->type != Invalid && prec < precedence(parser->currentToken)) {
    int validInfix = checkParseInfix(parser->currentToken);

    if(!validInfix) {
      return AST_NEW(Ast_empty);
    }

    leftExpression = parseInfix(parser, leftExpression);
    
  }

  return leftExpression;
}

Ast* parse(char* input) {
  printf("parsing: %s ...\n", input);

  LEXER* lexer = createLexer(input);
  PARSER* parser = createParser(lexer);

  return parseExpression(parser, 0);
}
