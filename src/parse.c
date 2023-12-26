#include "parse.h"
#include "token.h"
#include "tree.h"
#include "lexer.h"
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

void consumeToken(PARSER* parser) {
  parser->currentToken = parser->nextToken;
  parser->nextToken = nextToken(parser->lexer);
}


int checkParsePrefix(TOKEN* token) {
  switch (token->type) {
    case(Identifier):;
    case(BoolTrue):;
    case(BoolFalse):;
    case(Not):;

    break;
    default: return 0;
  }
  return 1;
}

int checkParseInfix(TOKEN* token) {
  switch (token->type) {
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

    default:;
  }
  return AST_NEW(Ast_empty);
}

Ast* parseInfix(PARSER* parser) {
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
  Ast* rightTree = parseExpression(parser);
  return AST_NEW(Ast_not, rightTree);
}

//Parser
Ast* parseExpression(PARSER* parser) {
  int validPrefix = checkParsePrefix(parser->currentToken);
  int valigInfix = checkParseInfix(parser->currentToken);

  if(validPrefix) {
    return parsePrefix(parser);
  }

  return AST_NEW(Ast_empty);
}
