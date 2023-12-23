#include "parse.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  enum {
    PARSE_RESULT,
    PARSE_ERROR,
  } type;
  union {
    struct PARSE_RESULT {Ast* parsetree; char* restString;} PARSE_RESULT;
    struct PARSE_ERROR {} PARSE_ERROR;
  } data;
} OPTIONAL_PARSE;

OPTIONAL_PARSE* new_opt_parse(OPTIONAL_PARSE optional_parse);

OPTIONAL_PARSE* new_opt_parse(OPTIONAL_PARSE optional_parse) {
  OPTIONAL_PARSE *ptr = malloc(sizeof(OPTIONAL_PARSE));
  if (ptr) *ptr = optional_parse;
  return ptr;
}

#define NEW_OPT_PARSE(type, ...) \
  new_opt_parse((OPTIONAL_PARSE){type, {.type=(struct type){__VA_ARGS__}}})

int hasResult(OPTIONAL_PARSE *opt_parse) {
  return opt_parse->type == PARSE_RESULT;
}

Ast* getAst(OPTIONAL_PARSE *opt_parse) {
  return opt_parse->data.PARSE_RESULT.parsetree;
}

Ast* getAstSafe(OPTIONAL_PARSE *opt_parse) {
  if (hasResult(opt_parse)) return getAst(opt_parse);
  return AST_NEW(Ast_empty);
}

char* getRestString(OPTIONAL_PARSE *opt_parse) {
  return opt_parse->data.PARSE_RESULT.restString;
}

char* getRestStringSafe(OPTIONAL_PARSE *opt_parse) {
  if (hasResult(opt_parse)) return getRestString(opt_parse);
  return "";
}

void test() {
  OPTIONAL_PARSE* newvar = NEW_OPT_PARSE(PARSE_ERROR);
}
