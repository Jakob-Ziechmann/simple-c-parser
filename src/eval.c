#include "eval.h"
#include "map.h"
#include "tree.h"


int evaluateIdentifier(Ast* tree, Map* assignment) {
  if(tree->tag != Ast_identifier) return -1;
  char* idName = tree->data.Ast_identifier.identifier;

  int value = getFromMap(assignment, idName);
  return value;
}

int evaluateAnd(Ast* tree, Map* assignment) {
  if(tree->tag != Ast_and) return -1;
  Ast* left = tree->data.Ast_and.left;
  Ast* right = tree->data.Ast_and.right;

  return evaluateTree(left, assignment) && evaluateTree(right, assignment);
}

int evaluateOr(Ast* tree, Map* assignment) {
  if(tree->tag != Ast_or) return -1;
  Ast* left = tree->data.Ast_or.left;
  Ast* right = tree->data.Ast_or.right;

  return evaluateTree(left, assignment) || evaluateTree(right, assignment);
}

int evaluateLeftImplication(Ast* tree, Map* assignment) {
  if(tree->tag != Ast_li) return -1;
  Ast* left = tree->data.Ast_li.left;
  Ast* right = tree->data.Ast_li.right;

  return evaluateTree(left, assignment) || (!evaluateTree(right, assignment));
}

int evaluateRightImplication(Ast* tree, Map* assignment) {
  if(tree->tag != Ast_ri) return -1;
  Ast* left = tree->data.Ast_ri.left;
  Ast* right = tree->data.Ast_ri.right;

  return (!evaluateTree(left, assignment)) || evaluateTree(right, assignment);
}

int evaluateEquivalence(Ast* tree, Map* assignment) {
  if(tree->tag != Ast_eq) return -1;
  Ast* left = tree->data.Ast_eq.left;
  Ast* right = tree->data.Ast_eq.right;

  return evaluateTree(left, assignment) == evaluateTree(right, assignment); 
}

int evaluateNot(Ast* tree, Map* assignment) {
  if(tree->tag != Ast_not) return -1;

  return !evaluateTree(tree->data.Ast_not.right, assignment);
}

int evaluateTree(Ast* tree, Map* assignment) {
  switch (tree->tag) {
    case(Ast_identifier): return evaluateIdentifier(tree, assignment);
    case(Ast_literal_true): return 1;
    case(Ast_literal_false): return 0;
    case(Ast_and): return evaluateAnd(tree, assignment);
    case(Ast_or): return evaluateOr(tree, assignment);
    case(Ast_li): return evaluateLeftImplication(tree, assignment);
    case(Ast_ri): return evaluateRightImplication(tree, assignment);
    case(Ast_eq): return evaluateEquivalence(tree, assignment);
    case(Ast_not): return evaluateNot(tree, assignment);
    default: return 0;
  }
}
