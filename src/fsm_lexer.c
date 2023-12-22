// This is an implementation of a FSM based on code of Gary Sims.
// gcc -O3 -o fsm-lexer fsm-lexer.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsm_lexer.h"

typedef enum {
  START,

  //infix operator
  INFIX_RI1,
  INFIX_LIEQ1,

  internal_END_STATES,

  //identifier
  IDENTIFIER_START,
  IDENTIFIER_REST,

  //literals
  __LITERAL,

  //delimiter
  __DELIMITER,

  //prefix operator
  __PREFIX_OPERATOR,

  //infix operator 
  INFIX_SINGLE,
  INFIX_LIEQ2,
  INFIX_EQ,
  INFIX_RI2,

  internal_LAST_STATE,
  TRAP,
} FSM_STATES;

typedef struct {
  FSM_STATES current_state;
  char *tokens;
  FSM_STATES next_state;
} FSM_TRANSITION_LOGIC;



FSM_STATES fsmTransition(FSM_STATES current_state, char *token, FSM_TRANSITION_LOGIC *state_machine) {
  FSM_TRANSITION_LOGIC *transition = state_machine;

  while (transition->current_state != internal_LAST_STATE) {
    if (transition->current_state == current_state) {
      if (strchr(transition->tokens, *token) != NULL) {
        return transition->next_state;
      }
    }
    transition++;
  }

  return TRAP;
}

int isAcceptingState(FSM_STATES state) {
  return (state > internal_END_STATES && state != TRAP);
}

// alpha = "abcdefghijklmnopqrstuvwxyz";
// number = "0123456789";
// alphanum = "abcdefghijklmnopqrstuvwxyz0123456789";

static FSM_TRANSITION_LOGIC fsm_lexer[] = {
  //Identifier
  {START, "abcdefghijklmnopqrstuvwxyz", IDENTIFIER_START},
  {IDENTIFIER_START, "abcdefghijklmnopqrstuvwxyz0123456789", IDENTIFIER_REST},
  {IDENTIFIER_REST, "abcdefghijklmnopqrstuvwxyz0123456789", IDENTIFIER_REST},
  
  //literals 
  {START, "01", __LITERAL},

  //dilimiter
  {START, "()", __DELIMITER},

  //prefix operator
  {START, "!", __PREFIX_OPERATOR},

  //infix operator
  {START, "*+", INFIX_SINGLE},
  {START, "<", INFIX_LIEQ1},
  {INFIX_LIEQ1, "=", INFIX_LIEQ2},
  {INFIX_LIEQ2, ">", INFIX_EQ},
  {START, "=", INFIX_RI1},
  {INFIX_RI1, ">", INFIX_RI2},

  {internal_LAST_STATE, "", internal_LAST_STATE}
};

static TOKEN errorToken = {ERROR, ""};

TOKEN_TYPE getTokenTypeOf(FSM_STATES state) {
  switch (state) {
    case(IDENTIFIER_START):
    case(IDENTIFIER_REST):
      return IDENTIFIER;

    case(__LITERAL):
      return LITERAL;

    case(__DELIMITER):
      return DELIMITER;

    case(__PREFIX_OPERATOR):
      return PREFIX_OPERATOR;

    case(INFIX_SINGLE):
    case(INFIX_LIEQ2):
    case(INFIX_RI2):
    case(INFIX_EQ):
      return INFIX_OPRATOR;

    default: return ERROR;
  }
}

char* tokenTypeToStr(TOKEN_TYPE tokenType) {
  switch (tokenType) {
    case(IDENTIFIER): return "Identifier";
    case(LITERAL): return "Literal";
    case(DELIMITER):  return "Delimiter";
    case(PREFIX_OPERATOR): return "Prefix Operator";
    case(INFIX_OPRATOR): return "Infix Operator";
    default: return "Error";
  }
}

struct Tuple {
  FSM_STATES state;
  int depth;
};

struct Tuple findLongestToken(FSM_STATES current_state, char* string) {
  char* c = string;
  char* nextc = c + 1;

  if(strlen(c) == 0) {
    struct Tuple finalTuple = {current_state, 0};
    return finalTuple;
  } 

  FSM_STATES next_state = fsmTransition(current_state, c, fsm_lexer);

  struct Tuple result = findLongestToken(next_state, c + 1);
  //when an accepting state came before came current state then the depth is 
  //incremented by one and the result gets returned
  if(isAcceptingState(result.state)) {
    result.depth++;
    return result;
  } 

  //when the current state is the last accepting state for the given string, 
  //then the state is returned and the depth is set to 0
  if(isAcceptingState(current_state)) {
    result.state = current_state;
    return result;
  }

  //this returns, if neighther this state is an accepting state nor the once 
  //coming after
  return result;
}

char* splitSringAfter(char* string, int length) {
  int i = 0;
  char* result = (char *)malloc(length + 1);
  while (i < length && string + i) {
    *(result + i) = *(string + i);
    i++;
  }

  result += '\0';
  return result;
}

TOKEN nextToken(char* string) {
  char* c = string;

  struct Tuple token_delim = findLongestToken(START, string);
  TOKEN_TYPE type = getTokenTypeOf(token_delim.state);
  char* token_string = splitSringAfter(string, token_delim.depth);
  char* rest_string = string + token_delim.depth;

  TOKEN token = {type, token_string, rest_string};
  return token;  
}

char* tokenToStr(TOKEN token) {
  char* tokenType = tokenTypeToStr(token.tokenType);
  char* segment = ": ";
  char* tokenString = token.token;

  int lenght = strlen(tokenType) + strlen(segment) + strlen(tokenString);
  char* destination = (char *)malloc(lenght + 1);

  strcat(destination, tokenType);
  strcat(destination, segment);
  strcat(destination, tokenString);

  return destination;
}
